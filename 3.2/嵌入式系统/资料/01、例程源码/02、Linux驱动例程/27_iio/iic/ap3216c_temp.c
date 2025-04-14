/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: ap3216c.c
作者	  	: 正点原子Linux团队
版本	   	: V1.0
描述	   	: AP3216C驱动程序
其他	   	: 无
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2021/03/19 正点原子Linux团队创建
			 V1.1 2021/03/19
			 使用regmap来访问寄存器	
***************************************************************/
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of_gpio.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/i2c.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/regmap.h>
#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>
#include <linux/iio/trigger_consumer.h>
#include <linux/iio/buffer.h>
#include <linux/iio/triggered_buffer.h>
#include <linux/unaligned/be_byteshift.h>
#include <linux/iio/trigger.h>
#include "ap3216creg.h"


#define AP3216C_NAME			"ap3216c"
#define AP3216C_PSINT_STATE		0x02
#define AP3216C_ALSINT_STATE	0x01

/* 
 * AP3216C的扫描元素，1路ALS(环境关)，1路PS(距离传感器)，1路IR
 */
enum inv_icm20608_scan {
	AP3216C_ALS,
	AP3216C_PS,
	AP3216C_IR,
};

/* 
 * ap3216c环境光传感器分辨率,扩大1000000倍,
 * 量程依次为0～20661，0～5162，0～1291，0～323。单位：lux
 */
static const int als_scale_ap3216c[] = {315000, 78800, 19700, 4900};

struct ap3216c_dev {
	struct i2c_client *client;	/* i2c 设备 */
	struct regmap *regmap;				/* regmap */
	struct regmap_config regmap_config;	
	struct mutex lock;
	struct iio_trigger  *trig;
};

/*
 * ap3216c通道，1路ALS(环境关)，1路PS(距离传感器)，1路IR
 */
static const struct iio_chan_spec ap3216c_channels[] = {
	/* ALS通道 */
	{
		.type = IIO_INTENSITY,
		.modified = 1,
		.channel2 = IIO_MOD_LIGHT_BOTH,
		.address = AP3216C_ALSDATALOW,
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) |
			BIT(IIO_CHAN_INFO_SCALE),
		.scan_index = AP3216C_ALS,
		.scan_type = {
			.sign = 'u',
			.realbits = 16,
			.storagebits = 16,
			.endianness = IIO_LE,
		},
	},

	/* PS通道 */
	{
		.type = IIO_PROXIMITY,
		.address = AP3216C_PSDATALOW,
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),
		.scan_index = AP3216C_PS,
		.scan_type = {
			.sign = 'u',
			.realbits = 10,
			.storagebits = 16,
			.endianness = IIO_LE,
		},
	},

	/* IR通道 */
	{
		.type = IIO_INTENSITY,
		.modified = 1,
		.channel2 = IIO_MOD_LIGHT_IR,
		.address = AP3216C_IRDATALOW,
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),
		.scan_index = AP3216C_IR,
		.scan_type = {
			.sign = 'u',
			.realbits = 10,
			.storagebits = 16,
			.endianness = IIO_LE,
		},
	},
};

/*
 * 扫描掩码，两种情况，全启动0X111，或者都不启动0X0
 */
static const unsigned long ap3216c_scan_masks[] = {
	BIT(AP3216C_ALS)
	| BIT(AP3216C_PS)
	| BIT(AP3216C_IR),
	0,
};

/*
 * @description	: 读取ap3216c指定寄存器值，读取一个寄存器
 * @param - dev:  ap3216c设备
 * @param - reg:  要读取的寄存器
 * @return 	  :   读取到的寄存器值
 */
static unsigned char ap3216c_read_reg(struct ap3216c_dev *dev, u8 reg)
{
	u8 ret;
	unsigned int data;

	ret = regmap_read(dev->regmap, reg, &data);
	return (u8)data;
}

/*
 * @description	: 向ap3216c指定寄存器写入指定的值，写一个寄存器
 * @param - dev:  ap3216c设备
 * @param - reg:  要写的寄存器
 * @param - data: 要写入的值
 * @return   :    无
 */
static void ap3216c_write_reg(struct ap3216c_dev *dev, u8 reg, u8 data)
{
	regmap_write(dev->regmap, reg, data);
}

/*
 * @description		: 初始化AP3216C
 * @param - dev 	: 要初始化的ap3216c设备
 * @return 			: 0 成功;其他 失败
 */
static int ap3216c_reginit(struct ap3216c_dev *dev)
{
	/* 初始化AP3216C */
	ap3216c_write_reg(dev, AP3216C_SYSTEMCONG, 0x04);		/* 复位AP3216C 			*/
	mdelay(50);												/* AP3216C复位最少10ms 	*/
	ap3216c_write_reg(dev, AP3216C_SYSTEMCONG, 0X03);		/* 开启ALS、PS+IR 		*/
	ap3216c_write_reg(dev, AP3216C_ALSCONFIG, 0X00);		/* ALS单次转换触发中断，量程为0～20661 lux */
	ap3216c_write_reg(dev, AP3216C_PSLEDCONFIG, 0X13);		/* IR LED 1脉冲，驱动电流100%*/
	ap3216c_write_reg(dev, AP3216C_INTCLEAR, 0X00);			/* 设置ALS和PS中断为读清零 */
	ap3216c_write_reg(dev, AP3216C_PSCONFIG, 0X05);			/* 设置PS 2次转换触发中断,增益为2 */

	/* 设置ALS中断触发门限，大于0，小于65535就触发 */
	ap3216c_write_reg(dev, AP3216C_ALS_LOWTHRE_L, 0Xff);
	ap3216c_write_reg(dev, AP3216C_ALS_LOWTHRE_H, 0Xff);
	ap3216c_write_reg(dev, AP3216C_ALS_HIGTHRE_L, 0X00);
	ap3216c_write_reg(dev, AP3216C_ALS_HIGTHRE_H, 0X00);			


	ap3216c_read_reg(dev, AP3216C_ALSDATAHIGH);
	ap3216c_read_reg(dev, AP3216C_PSDATAHIGH);

	return 0;
}

/*
  * @description  	: 读取AP3216C传感器数
  * @param - dev	: ap3216c设备 
  * @param - reg  	: 要读取的通道寄存器首地址。
  * @param - chann2 : 需要读取的通道，比如ALS，IR。
  * @param - val  	: 保存读取到的值。
  * @return			: 0，成功；其他值，错误
  */
static int ap3216c_read_alsir_data(struct ap3216c_dev *dev, int reg,
				   int chann2, int *val)
{
	int ret = 0;
	unsigned char data[2];

	switch (chann2) {
	case IIO_MOD_LIGHT_BOTH:	/* 读取ALS数据 */
		ret = regmap_bulk_read(dev->regmap, reg, data, 2);
		*val = ((int)data[1] << 8) | data[0];   
		break;
	case IIO_MOD_LIGHT_IR:		/* 读取IR数据 */
		ret = regmap_bulk_read(dev->regmap, reg, data, 2);
		*val = ((int)data[1] << 2) | (data[0] & 0X03); 
		break;
	default:
		ret = -EINVAL;
		break;
	}

	if (ret) {
		return -EINVAL;
	}
		
	return IIO_VAL_INT;
}
/*
  * @description  	: 设置AP3216C的ALS量程(分辨率)
  * @param - dev	: ap3216c设备
  * @param - val   	: 量程(分辨率值)。
  * @param - chann2 : 需要设置的通道。
  * @return			: 0，成功；其他值，错误
  */
static int ap3216c_write_als_scale(struct ap3216c_dev *dev, int chann2, int val)
{
	int ret = 0, i;	
	u8 d;

	switch (chann2) {
	case IIO_MOD_LIGHT_BOTH:	/* 设置ALS分辨率 */
		for (i = 0; i < ARRAY_SIZE(als_scale_ap3216c); ++i) {
			if (als_scale_ap3216c[i] == val) {
				d = (i << 4);
				ret = regmap_write(dev->regmap, AP3216C_ALSCONFIG, d);
			}
		}
		break;
	default:
		ret = -EINVAL;
		break;
	}
		
	return ret;
}

/*
  * @description     	: 读函数，当读取sysfs中的文件的时候最终此函数会执行，此函数
  * 					：里面会从传感器里面读取各种数据，然后上传给应用。
  * @param - indio_dev	: iio_dev
  * @param - chan   	: 通道
  * @param - val   		: 读取的值，如果是小数值的话，val是整数部分。
  * @param - val2   	: 读取的值，如果是小数值的话，val2是小数部分。
  * @return				: 0，成功；其他值，错误
  */
static int ap3216c_read_raw(struct iio_dev *indio_dev,
			   struct iio_chan_spec const *chan,
			   int *val, int *val2, long mask)
{
	int ret = 0;
	unsigned char data[2];
	unsigned char regdata = 0;
	struct ap3216c_dev *dev = iio_priv(indio_dev);

	switch (mask) {
	case IIO_CHAN_INFO_RAW:								/* 读取ICM20608加速度计、陀螺仪、温度传感器原始值 */
		mutex_lock(&dev->lock);								/* 上锁 			*/
		switch (chan->type) {
		case IIO_INTENSITY:
			ret = ap3216c_read_alsir_data(dev, chan->address, chan->channel2, val); /* 读取ALS */
			break;				/* 值为val */
		case IIO_PROXIMITY:
			ret = regmap_bulk_read(dev->regmap, chan->address, data, 2);
			*val = ((int)(data[1] & 0X3F) << 4) | (data[0] & 0X0F);  
			ret = IIO_VAL_INT; 	/* 值为val */
			break;
		default:
			ret = -EINVAL;
			break;
		}
		mutex_unlock(&dev->lock);							/* 释放锁 			*/
		return ret;
	case IIO_CHAN_INFO_SCALE:
		switch (chan->type) {
		case IIO_INTENSITY:			/* ALS量程 */
			mutex_lock(&dev->lock);
			regdata = (ap3216c_read_reg(dev, AP3216C_ALSCONFIG) & 0X30) >> 4;
			*val  = 0;
			*val2 = als_scale_ap3216c[regdata];
			mutex_unlock(&dev->lock);
			return IIO_VAL_INT_PLUS_MICRO;	/* 值为val+val2/1000000 */
		default:
			return -EINVAL;
		}
		return ret;
		
	default:
		return -EINVAL;
	}
	return ret;
}

 /* @description     	: 写函数，当向sysfs中的文件写数据的时候最终此函数会执行，一般在此函数
  * 					：里面设置传感器，比如量程等。
  * @param - indio_dev	: iio_dev
  * @param - chan   	: 通道
  * @param - val   		: 应用程序写入的值，如果是小数值的话，val是整数部分。
  * @param - val2   	: 应用程序写入的值，如果是小数值的话，val2是小数部分。
  * @return				: 0，成功；其他值，错误
  */
static int ap3216c_write_raw(struct iio_dev *indio_dev,
			    struct iio_chan_spec const *chan,
			    int val, int val2, long mask)
{
	int ret = 0;
	struct ap3216c_dev *dev = iio_priv(indio_dev);

	iio_device_claim_direct_mode(indio_dev);		/* 保持direct模式 	*/
	switch (mask) {
	case IIO_CHAN_INFO_SCALE:	/* 设置ALS量程 */
		switch (chan->type) {
		case IIO_INTENSITY:		/* 设置ALS量程 */
			mutex_lock(&dev->lock);
			ret = ap3216c_write_als_scale(dev, chan->channel2, val2);
			mutex_unlock(&dev->lock);
			break;
		default:
			ret = -EINVAL;
			break;
		}
		break;
	
	default:
		ret = -EINVAL;
		break;
	}

	iio_device_release_direct_mode(indio_dev);			/* 释放direct模式 	*/
	return ret;
}

/*
  * @description     	: 用户空间写数据格式，比如我们在用户空间操作sysfs来设置传感器的分辨率，
  * 					：如果分辨率带小数，那么这个小数传递到内核空间应该扩大多少倍，此函数就是
  *						: 用来设置这个的。
  * @param - indio_dev	: iio_dev
  * @param - chan   	: 通道
  * @param - mask   	: 掩码
  * @return				: 0，成功；其他值，错误
  */
static int ap3216c_write_raw_get_fmt(struct iio_dev *indio_dev,
				 struct iio_chan_spec const *chan, long mask)
{
	switch (mask) {
	case IIO_CHAN_INFO_SCALE:
		switch (chan->type) {
		case IIO_INTENSITY:		/* 用户空间写的陀螺仪分辨率数据要乘以1000000 */
			return IIO_VAL_INT_PLUS_MICRO;
		default:				
			return IIO_VAL_INT_PLUS_MICRO;
		}
	default:
		return IIO_VAL_INT_PLUS_MICRO;
	}

	return -EINVAL;
}

/*
 * iio_info结构体变量
 */
static const struct iio_info ap3216c_info = {
	.read_raw		= ap3216c_read_raw,
	.write_raw		= ap3216c_write_raw,
	.write_raw_get_fmt = &ap3216c_write_raw_get_fmt,	/* 用户空间写数据格式 */
};

/*
 * @description ：中断服务函数，对于iio触发器来说，一般在此函数
 *	              里面直接调用iio_trigger_poll
 */
irqreturn_t iio_trigger_generic_data_rdy_poll(int irq, void *private)
{
	iio_trigger_poll(private);
	return IRQ_HANDLED;
}

/*
  * @description     : 触发器下半部函数，从启动的通道里面读取数据，
  *                    然后把这些数据送入缓冲区。
  * @param - irq  	 : 中断号
  * @param - p   	 : 为iio_poll_func结构体类型指针变量，iio_poll_func里面有iio_dev
  */
irqreturn_t ap3216c_trigger_handler(int irq, void *p)
{
	int ret = 0;
	struct iio_poll_func *pf = p;
	struct iio_dev *indio_dev = pf->indio_dev;
	struct ap3216c_dev *dev = iio_priv(indio_dev);
	int int_status = 0;
	u8 data[6];
	int regdata = 0;
	u8 i = 0;

	printk("ap3216c_trigger_handler\r\n");

	mutex_lock(&dev->lock);

	regdata = ap3216c_read_reg(dev, AP3216C_INTSTATUS);
	printk("ap3216c int status = %#X\r\n", regdata);
	if(regdata & AP3216C_ALSINT_STATE) {
		printk("ap3216c als int\r\n");
	}	
	if(regdata & AP3216C_PSINT_STATE) {
		printk("ap3216c ps int\r\n");
	}	

	/* 循环读取所有传感器数据 */
    for(i = 0; i < 6; i++) {
        data[i] = ap3216c_read_reg(dev, AP3216C_IRDATALOW + i);	
		//ret = regmap_read(dev->regmap, AP3216C_IRDATALOW + i, data[i]);
    }

	if (ret)
			goto end_session;
	iio_push_to_buffers_with_timestamp(indio_dev, data, pf->timestamp);

end_session:
	mutex_unlock(&dev->lock);
	iio_trigger_notify_done(indio_dev->trig);

	return IRQ_HANDLED;
}

/*
  * @description     : 触发器开关，一般在此函数里面实现设备的打开和关闭操作
  *                    比如使能/关闭中断等
  * @param - trig  	 : 触发器
  * @param - state   : 状态，true，打开；flase，关闭。
  * @return   	 	 : 0,成功；其他值,失败
  * 
  */
static int ap3216c_trigger_set_state(struct iio_trigger *trig,
					      bool state)
{
	int ret = 0;
	struct iio_dev *indio_dev = iio_trigger_get_drvdata(trig);
	struct ap3216c_dev *dev = iio_priv(indio_dev);

	mutex_lock(&dev->lock);
	if (state) {
		
	} else {
		
	}
	mutex_unlock(&dev->lock);

	return ret;
}


/*
 * 触发器操作函数集
 */
static const struct iio_trigger_ops ap3216c_trigger_ops = {
	.set_trigger_state = &ap3216c_trigger_set_state,
};

 /*
  * @description     : i2c驱动的probe函数，当驱动与
  *                    设备匹配以后此函数就会执行
  * @param - client  : i2c设备
  * @param - id      : i2c设备ID
  * @return          : 0，成功;其他负值,失败
  */
static int ap3216c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;
	struct ap3216c_dev *dev;
	struct iio_dev *indio_dev;

	/*  1、申请iio_dev内存 */
	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*dev));
	if (!indio_dev)
		return -ENOMEM;

	/* 2、获取ap3216c_dev结构体地址 */
	dev = iio_priv(indio_dev); 
	dev->client = client;
	
	i2c_set_clientdata(client, indio_dev); /* 保存ap3216cdev结构体 */
		
	/* 初始化regmap_config设置 */
	dev->regmap_config.reg_bits = 8;		/* 寄存器长度8bit */
	dev->regmap_config.val_bits = 8;		/* 值长度8bit */

	/* 初始化IIC接口的regmap */
	dev->regmap = regmap_init_i2c(client, &dev->regmap_config);
	if (IS_ERR(dev->regmap)) {
		ret = PTR_ERR(dev->regmap);
		goto err_regmap_init;
	}	

	mutex_init(&dev->lock);	

	/* 4、iio_dev的其他成员变量 */
	indio_dev->dev.parent = &client->dev;
	indio_dev->info = &ap3216c_info;
	indio_dev->name = AP3216C_NAME;	
	indio_dev->modes = INDIO_DIRECT_MODE;	/* 直接模式，提供sysfs接口 */
	indio_dev->channels = ap3216c_channels;
	indio_dev->num_channels = ARRAY_SIZE(ap3216c_channels);
	indio_dev->available_scan_masks = ap3216c_scan_masks;

	/* 5、触发缓冲区设置 */
	ret = devm_iio_triggered_buffer_setup(&client->dev, indio_dev,
						 iio_pollfunc_store_time,
						 ap3216c_trigger_handler,
						 NULL);

	/* 6、申请trigger,并初始化 */
	dev->trig = devm_iio_trigger_alloc(&indio_dev->dev,
					  "%s-dev%d",
					  indio_dev->name,
					  indio_dev->id);
	if (!dev->trig){
		ret = -ENOMEM;
		goto err_iio_trriger_alloc;
	}

	dev->trig->dev.parent = regmap_get_device(dev->regmap);
	dev->trig->ops = &ap3216c_trigger_ops;
	iio_trigger_set_drvdata(dev->trig, indio_dev);

	/* 7、向内核注册触发器 */
	ret = devm_iio_trigger_register(&indio_dev->dev, dev->trig);
	indio_dev->trig = iio_trigger_get(dev->trig);

	/* 8、AP3216C中断初始化 */
	ret = devm_request_irq(&indio_dev->dev, client->irq,
			       &iio_trigger_generic_data_rdy_poll,
			       IRQF_TRIGGER_FALLING,
			       "ap3216c",
			       dev->trig);
	if (ret) {
		goto err_irq_request;
	}

	/* 9、注册iio_dev */
	ret = iio_device_register(indio_dev);
	if (ret < 0) {
		dev_err(&client->dev, "iio_device_register failed\n");
		goto err_iio_register;
	}

	ap3216c_reginit(dev); /* 初始化ap3216c */
	return 0;

err_irq_request:
err_iio_register:
err_iio_trriger_alloc:
err_regmap_init:
	iio_device_unregister(indio_dev);
	return ret;
}

/*
 * @description     : i2c驱动的remove函数，移除i2c驱动的时候此函数会执行
 * @param - client 	: i2c设备
 * @return          : 0，成功;其他负值,失败
 */
static int ap3216c_remove(struct i2c_client *client)
{
	struct iio_dev *indio_dev = i2c_get_clientdata(client);
	struct ap3216c_dev *dev;
	
	dev = iio_priv(indio_dev);

	/* 1、释放regmap */
	regmap_exit(dev->regmap);
	/* 2、注销IIO */
	iio_device_unregister(indio_dev);
	return 0;
}

/* 传统匹配方式ID列表 */
static const struct i2c_device_id ap3216c_id[] = {
	{"alientek,ap3216c", 0},  
	{}
};

/* 设备树匹配列表 */
static const struct of_device_id ap3216c_of_match[] = {
	{ .compatible = "alientek,ap3216c" },
	{ /* Sentinel */ }
};

/* i2c驱动结构体 */	
static struct i2c_driver ap3216c_driver = {
	.probe = ap3216c_probe,
	.remove = ap3216c_remove,
	.driver = {
			.owner = THIS_MODULE,
		   	.name = "ap3216c",
		   	.of_match_table = ap3216c_of_match, 
		   },
	.id_table = ap3216c_id,
};
		   
/*
 * @description	: 驱动入口函数
 * @param 		: 无
 * @return 		: 无
 */
static int __init ap3216c_init(void)
{
	int ret = 0;

	ret = i2c_add_driver(&ap3216c_driver);
	return ret;
}

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static void __exit ap3216c_exit(void)
{
	i2c_del_driver(&ap3216c_driver);
}

/* module_i2c_driver(ap3216c_driver) */

module_init(ap3216c_init);
module_exit(ap3216c_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ALIENTEK");
MODULE_INFO(intree, "Y");
