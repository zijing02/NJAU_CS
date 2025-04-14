#include <linux/module.h>
#include <linux/ratelimit.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/input/mt.h>
#include <linux/input/touchscreen.h>
#include <linux/input/edt-ft5x06.h>
#include <linux/i2c.h>
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: ft5x06.c
作者	  	: 左忠凯
版本	   	: V1.0
描述	   	: FT5X06，包括FT5206、FT5426等触摸屏驱动程序
其他	   	: 无
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/12/23 左忠凯创建个
***************************************************************/

#define MAX_SUPPORT_POINTS		5			/* 5点触摸 	*/
#define TOUCH_EVENT_DOWN		0x00		/* 按下 	*/
#define TOUCH_EVENT_UP			0x01		/* 抬起 	*/
#define TOUCH_EVENT_ON			0x02		/* 接触 	*/
#define TOUCH_EVENT_RESERVED	0x03		/* 保留 	*/

/* FT5X06寄存器相关宏定义 */
#define FT5X06_TD_STATUS_REG	0X02		/*	状态寄存器地址 		*/
#define FT5x06_DEVICE_MODE_REG	0X00 		/* 模式寄存器 			*/
#define FT5426_IDG_MODE_REG		0XA4		/* 中断模式				*/
#define FT5X06_READLEN			29			/* 要读取的寄存器个数 	*/

struct ft5x06_dev {
	struct device_node	*nd; 				/* 设备节点 		*/
	int irq_pin,reset_pin;					/* 中断和复位IO		*/
	int irqnum;								/* 中断号    		*/
	void *private_data;						/* 私有数据 		*/
	struct input_dev *input;				/* input结构体 		*/
	struct i2c_client *client;				/* I2C客户端 		*/
};

static struct ft5x06_dev ft5x06;

/*
 * @description     : 复位FT5X06
 * @param - client 	: 要操作的i2c
 * @param - multidev: 自定义的multitouch设备
 * @return          : 0，成功;其他负值,失败
 */
static int ft5x06_ts_reset(struct i2c_client *client, struct ft5x06_dev *dev)
{
	int ret = 0;

	if (gpio_is_valid(dev->reset_pin)) {  		/* 检查IO是否有效 */
		/* 申请复位IO，并且默认输出低电平 */
		ret = devm_gpio_request_one(&client->dev,	
					dev->reset_pin, GPIOF_OUT_INIT_LOW,
					"edt-ft5x06 reset");
		if (ret) {
			return ret;
		}

		msleep(5);
		gpio_set_value(dev->reset_pin, 1);	/* 输出高电平，停止复位 */
		msleep(300);
	}

	return 0;
}

/*
 * @description	: 从FT5X06读取多个寄存器数据
 * @param - dev:  ft5x06设备
 * @param - reg:  要读取的寄存器首地址
 * @param - val:  读取到的数据
 * @param - len:  要读取的数据长度
 * @return 		: 操作结果
 */
static int ft5x06_read_regs(struct ft5x06_dev *dev, u8 reg, void *val, int len)
{
	int ret;
	struct i2c_msg msg[2];
	struct i2c_client *client = (struct i2c_client *)dev->client;

	/* msg[0]为发送要读取的首地址 */
	msg[0].addr = client->addr;			/* ft5x06地址 */
	msg[0].flags = 0;					/* 标记为发送数据 */
	msg[0].buf = &reg;					/* 读取的首地址 */
	msg[0].len = 1;						/* reg长度*/

	/* msg[1]读取数据 */
	msg[1].addr = client->addr;			/* ft5x06地址 */
	msg[1].flags = I2C_M_RD;			/* 标记为读取数据*/
	msg[1].buf = val;					/* 读取数据缓冲区 */
	msg[1].len = len;					/* 要读取的数据长度*/

	ret = i2c_transfer(client->adapter, msg, 2);
	if(ret == 2) {
		ret = 0;
	} else {
		ret = -EREMOTEIO;
	}
	return ret;
}

/*
 * @description	: 向ft5x06多个寄存器写入数据
 * @param - dev:  ft5x06设备
 * @param - reg:  要写入的寄存器首地址
 * @param - val:  要写入的数据缓冲区
 * @param - len:  要写入的数据长度
 * @return 	  :   操作结果
 */
static s32 ft5x06_write_regs(struct ft5x06_dev *dev, u8 reg, u8 *buf, u8 len)
{
	u8 b[256];
	struct i2c_msg msg;
	struct i2c_client *client = (struct i2c_client *)dev->client;
	
	b[0] = reg;					/* 寄存器首地址 */
	memcpy(&b[1],buf,len);		/* 将要写入的数据拷贝到数组b里面 */
		
	msg.addr = client->addr;	/* ft5x06地址 */
	msg.flags = 0;				/* 标记为写数据 */

	msg.buf = b;				/* 要写入的数据缓冲区 */
	msg.len = len + 1;			/* 要写入的数据长度 */

	return i2c_transfer(client->adapter, &msg, 1);
}

/*
 * @description	: 向ft5x06指定寄存器写入指定的值，写一个寄存器
 * @param - dev:  ft5x06设备
 * @param - reg:  要写的寄存器
 * @param - data: 要写入的值
 * @return   :    无
 */
static void ft5x06_write_reg(struct ft5x06_dev *dev, u8 reg, u8 data)
{
	u8 buf = 0;
	buf = data;
	ft5x06_write_regs(dev, reg, &buf, 1);
}

/*
 * @description     : FT5X06中断服务函数
 * @param - irq 	: 中断号 
 * @param - dev_id	: 设备结构。
 * @return 			: 中断执行结果
 */
static irqreturn_t ft5x06_handler(int irq, void *dev_id)
{
	struct ft5x06_dev *multidata = dev_id;

	u8 rdbuf[29];
	int i, type, x, y, id;
	int offset, tplen;
	int ret;
	bool down;

	offset = 1; 	/* 偏移1，也就是0X02+1=0x03,从0X03开始是触摸值 */
	tplen = 6;		/* 一个触摸点有6个寄存器来保存触摸值 */

	memset(rdbuf, 0, sizeof(rdbuf));		/* 清除 */

	/* 读取FT5X06触摸点坐标从0X02寄存器开始，连续读取29个寄存器 */
	ret = ft5x06_read_regs(multidata, FT5X06_TD_STATUS_REG, rdbuf, FT5X06_READLEN);
	if (ret) {
		goto fail;
	}

	/* 上报每一个触摸点坐标 */
	for (i = 0; i < MAX_SUPPORT_POINTS; i++) {
		u8 *buf = &rdbuf[i * tplen + offset];

		/* 以第一个触摸点为例，寄存器TOUCH1_XH(地址0X03),各位描述如下：
		 * bit7:6  Event flag  0:按下 1:释放 2：接触 3：没有事件
		 * bit5:4  保留
		 * bit3:0  X轴触摸点的11~8位。
		 */
		type = buf[0] >> 6;     /* 获取触摸类型 */
		if (type == TOUCH_EVENT_RESERVED)
			continue;
 
		/* 我们所使用的触摸屏和FT5X06是反过来的 */
		x = ((buf[2] << 8) | buf[3]) & 0x0fff;
		y = ((buf[0] << 8) | buf[1]) & 0x0fff;
		
		/* 以第一个触摸点为例，寄存器TOUCH1_YH(地址0X05),各位描述如下：
		 * bit7:4  Touch ID  触摸ID，表示是哪个触摸点
		 * bit3:0  Y轴触摸点的11~8位。
		 */
		id = (buf[2] >> 4) & 0x0f;
		down = type != TOUCH_EVENT_UP;

		input_mt_slot(multidata->input, id);
		input_mt_report_slot_state(multidata->input, MT_TOOL_FINGER, down);

		if (!down)
			continue;

		input_report_abs(multidata->input, ABS_MT_POSITION_X, x);
		input_report_abs(multidata->input, ABS_MT_POSITION_Y, y);
	}

	input_mt_report_pointer_emulation(multidata->input, true);
	input_sync(multidata->input);

fail:
	return IRQ_HANDLED;

}

/*
 * @description     : FT5x06中断初始化
 * @param - client 	: 要操作的i2c
 * @param - multidev: 自定义的multitouch设备
 * @return          : 0，成功;其他负值,失败
 */
static int ft5x06_ts_irq(struct i2c_client *client, struct ft5x06_dev *dev)
{
	int ret = 0;

	/* 1,申请中断GPIO */
	if (gpio_is_valid(dev->irq_pin)) {
		ret = devm_gpio_request_one(&client->dev, dev->irq_pin,
					GPIOF_IN, "edt-ft5x06 irq");
		if (ret) {
			dev_err(&client->dev,
				"Failed to request GPIO %d, error %d\n",
				dev->irq_pin, ret);
			return ret;
		}
	}

	/* 2，申请中断,client->irq就是IO中断， */
	ret = devm_request_threaded_irq(&client->dev, client->irq, NULL,
					ft5x06_handler, IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
					client->name, &ft5x06);
	if (ret) {
		dev_err(&client->dev, "Unable to request touchscreen IRQ.\n");
		return ret;
	}

	return 0;
}

 /*
  * @description     : i2c驱动的probe函数，当驱动与
  *                    设备匹配以后此函数就会执行
  * @param - client  : i2c设备
  * @param - id      : i2c设备ID
  * @return          : 0，成功;其他负值,失败
  */
static int ft5x06_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret = 0;

	ft5x06.client = client;

	/* 1，获取设备树中的中断和复位引脚 */
	ft5x06.irq_pin = of_get_named_gpio(client->dev.of_node, "interrupt-gpios", 0);
	ft5x06.reset_pin = of_get_named_gpio(client->dev.of_node, "reset-gpios", 0);

	/* 2，复位FT5x06 */
	ret = ft5x06_ts_reset(client, &ft5x06);
	if(ret < 0) {
		goto fail;
	}

	/* 3，初始化中断 */
	ret = ft5x06_ts_irq(client, &ft5x06);
	if(ret < 0) {
		goto fail;
	}

	/* 4，初始化FT5X06 */
	ft5x06_write_reg(&ft5x06, FT5x06_DEVICE_MODE_REG, 0); 	/* 进入正常模式 	*/
	ft5x06_write_reg(&ft5x06, FT5426_IDG_MODE_REG, 1); 		/* FT5426中断模式	*/

	/* 5，input设备注册 */
	ft5x06.input = devm_input_allocate_device(&client->dev);
	if (!ft5x06.input) {
		ret = -ENOMEM;
		goto fail;
	}
	ft5x06.input->name = client->name;
	ft5x06.input->id.bustype = BUS_I2C;
	ft5x06.input->dev.parent = &client->dev;

	__set_bit(EV_KEY, ft5x06.input->evbit);
	__set_bit(EV_ABS, ft5x06.input->evbit);
	__set_bit(BTN_TOUCH, ft5x06.input->keybit);

	input_set_abs_params(ft5x06.input, ABS_X, 0, 1024, 0, 0);
	input_set_abs_params(ft5x06.input, ABS_Y, 0, 600, 0, 0);
	input_set_abs_params(ft5x06.input, ABS_MT_POSITION_X,0, 1024, 0, 0);
	input_set_abs_params(ft5x06.input, ABS_MT_POSITION_Y,0, 600, 0, 0);	     
	ret = input_mt_init_slots(ft5x06.input, MAX_SUPPORT_POINTS, 0);
	if (ret) {
		goto fail;
	}

	ret = input_register_device(ft5x06.input);
	if (ret)
		goto fail;

	return 0;

fail:
	return ret;
}

/*
 * @description     : i2c驱动的remove函数，移除i2c驱动的时候此函数会执行
 * @param - client 	: i2c设备
 * @return          : 0，成功;其他负值,失败
 */
static int ft5x06_ts_remove(struct i2c_client *client)
{	
	/* 释放input_dev */
	input_unregister_device(ft5x06.input);
	return 0;
}


/*
 *  传统驱动匹配表
 */ 
static const struct i2c_device_id ft5x06_ts_id[] = {
	{ "edt-ft5206", 0, },
	{ "edt-ft5426", 0, },
	{ /* sentinel */ }
};

/*
 * 设备树匹配表 
 */
static const struct of_device_id ft5x06_of_match[] = {
	{ .compatible = "edt,edt-ft5206", },
	{ .compatible = "edt,edt-ft5426", },
	{ /* sentinel */ }
};

/* i2c驱动结构体 */	
static struct i2c_driver ft5x06_ts_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "edt_ft5x06",
		.of_match_table = of_match_ptr(ft5x06_of_match),
	},
	.id_table = ft5x06_ts_id,
	.probe    = ft5x06_ts_probe,
	.remove   = ft5x06_ts_remove,
};

/*
 * @description	: 驱动入口函数
 * @param 		: 无
 * @return 		: 无
 */
static int __init ft5x06_init(void)
{
	int ret = 0;

	ret = i2c_add_driver(&ft5x06_ts_driver);

	return ret;
}

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static void __exit ft5x06_exit(void)
{
	i2c_del_driver(&ft5x06_ts_driver);
}

module_init(ft5x06_init);
module_exit(ft5x06_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zuozhongkai");

