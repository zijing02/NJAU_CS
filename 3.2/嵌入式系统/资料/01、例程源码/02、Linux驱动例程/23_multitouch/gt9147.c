#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/regmap.h>
#include <linux/gpio/consumer.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/input/mt.h>
#include <linux/input/touchscreen.h>
#include <linux/i2c.h>
#include <asm/unaligned.h>

#define GT_CTRL_REG 	        0X8040  /* GT9147控制寄存器         */
#define GT_MODSW_REG 	        0X804D  /* GT9147模式切换寄存器        */
#define GT_9xx_CFGS_REG 	        0X8047  /* GT9147配置起始地址寄存器    */
#define GT_1xx_CFGS_REG 	        0X8050  /* GT1151配置起始地址寄存器    */
#define GT_CHECK_REG 	        0X80FF  /* GT9147校验和寄存器       */
#define GT_PID_REG 		        0X8140  /* GT9147产品ID寄存器       */

#define GT_GSTID_REG 	        0X814E  /* GT9147当前检测到的触摸情况 */
#define GT_TP1_REG 		        0X814F  /* 第一个触摸点数据地址 */
#define GT_TP2_REG 		        0X8157	/* 第二个触摸点数据地址 */
#define GT_TP3_REG 		        0X815F  /* 第三个触摸点数据地址 */
#define GT_TP4_REG 		        0X8167  /* 第四个触摸点数据地址  */
#define GT_TP5_REG 		        0X816F	/* 第五个触摸点数据地址   */
#define MAX_SUPPORT_POINTS      5       /* 最多5点电容触摸 */

struct gt9147_dev {
	int irq_pin,reset_pin;					/* 中断和复位IO		*/
	int irqnum;								/* 中断号    		*/
	int irqtype;							/* 中断类型         */
	int max_x;								/* 最大横坐标   	*/
	int max_y; 								/* 最大纵坐标		*/
	void *private_data;						/* 私有数据 		*/
	struct input_dev *input;				/* input结构体 		*/
	struct i2c_client *client;				/* I2C客户端 		*/

};
struct gt9147_dev gt9147;

const u8 irq_table[] = {IRQ_TYPE_EDGE_RISING, IRQ_TYPE_EDGE_FALLING, IRQ_TYPE_LEVEL_LOW, IRQ_TYPE_LEVEL_HIGH};  /* 触发方式 */

/*
 * @description     : 复位GT9147
 * @param - client 	: 要操作的i2c
 * @param - multidev: 自定义的multitouch设备
 * @return          : 0，成功;其他负值,失败
 */
static int gt9147_ts_reset(struct i2c_client *client, struct gt9147_dev *dev)
{
	int ret = 0;

    /* 申请复位IO*/
	if (gpio_is_valid(dev->reset_pin)) {  		
		/* 申请复位IO，并且默认输出高电平 */
		ret = devm_gpio_request_one(&client->dev,	
					dev->reset_pin, GPIOF_OUT_INIT_HIGH,
					"gt9147 reset");
		if (ret) {
			return ret;
		}
	}

    /* 申请中断IO*/
	if (gpio_is_valid(dev->irq_pin)) {  		
		/* 申请复位IO，并且默认输出高电平 */
		ret = devm_gpio_request_one(&client->dev,	
					dev->irq_pin, GPIOF_OUT_INIT_HIGH,
					"gt9147 int");
		if (ret) {
			return ret;
		}
	}

    /* 4、初始化GT9147，要严格按照GT9147时序要求 */
    gpio_set_value(dev->reset_pin, 0); /* 复位GT9147 */
    msleep(10);
    gpio_set_value(dev->reset_pin, 1); /* 停止复位GT9147 */
    msleep(10);
    gpio_set_value(dev->irq_pin, 0);    /* 拉低INT引脚 */
    msleep(50);
    gpio_direction_input(dev->irq_pin); /* INT引脚设置为输入 */

	return 0;
}

/*
 * @description	: 从GT9147读取多个寄存器数据
 * @param - dev:  GT9147设备
 * @param - reg:  要读取的寄存器首地址
 * @param - buf:  读取到的数据
 * @param - len:  要读取的数据长度
 * @return 		: 操作结果
 */
static int gt9147_read_regs(struct gt9147_dev *dev, u16 reg, u8 *buf, int len)
{
	int ret;
    u8 regdata[2];
	struct i2c_msg msg[2];
	struct i2c_client *client = (struct i2c_client *)dev->client;
    
    /* GT9147寄存器长度为2个字节 */
    regdata[0] = reg >> 8;
    regdata[1] = reg & 0xFF;

	/* msg[0]为发送要读取的首地址 */
	msg[0].addr = client->addr;			/* ft5x06地址 */
	msg[0].flags = !I2C_M_RD;			/* 标记为发送数据 */
	msg[0].buf = &regdata[0];			/* 读取的首地址 */
	msg[0].len = 2;						/* reg长度*/

	/* msg[1]读取数据 */
	msg[1].addr = client->addr;			/* ft5x06地址 */
	msg[1].flags = I2C_M_RD;			/* 标记为读取数据*/
	msg[1].buf = buf;					/* 读取数据缓冲区 */
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
 * @description	: 向GT9147多个寄存器写入数据
 * @param - dev:  GT9147设备
 * @param - reg:  要写入的寄存器首地址
 * @param - val:  要写入的数据缓冲区
 * @param - len:  要写入的数据长度
 * @return 	  :   操作结果
 */
static s32 gt9147_write_regs(struct gt9147_dev *dev, u16 reg, u8 *buf, u8 len)
{
	u8 b[256];
	struct i2c_msg msg;
	struct i2c_client *client = (struct i2c_client *)dev->client;
	
	b[0] = reg >> 8;			/* 寄存器首地址低8位 */
    b[1] = reg & 0XFF;			/* 寄存器首地址高8位 */
	memcpy(&b[2],buf,len);		/* 将要写入的数据拷贝到数组b里面 */

	msg.addr = client->addr;	/* gt9147地址 */
	msg.flags = 0;				/* 标记为写数据 */

	msg.buf = b;				/* 要写入的数据缓冲区 */
	msg.len = len + 2;			/* 要写入的数据长度 */

	return i2c_transfer(client->adapter, &msg, 1);
}

static irqreturn_t gt9147_irq_handler(int irq, void *dev_id)
{
    int touch_num = 0;
    int input_x, input_y;
    int id = 0;
    int ret = 0;
    u8 data;
    u8 touch_data[5];
    struct gt9147_dev *dev = dev_id;

    ret = gt9147_read_regs(dev, GT_GSTID_REG, &data, 1);
    if (data == 0x00)  {     /* 没有触摸数据，直接返回 */
        goto fail;
    } else {                 /* 统计触摸点数据 */
        touch_num = data & 0x0f;
    }

    /* 由于GT9147没有硬件检测每个触摸点按下和抬起，因此每个触摸点的抬起和按
     * 下不好处理，尝试过一些方法，但是效果都不好，因此这里暂时使用单点触摸 
     */
    if(touch_num) {         /* 单点触摸按下 */
        gt9147_read_regs(dev, GT_TP1_REG, touch_data, 5);
        id = touch_data[0] & 0x0F;
        if(id == 0) {
            input_x  = touch_data[1] | (touch_data[2] << 8);
            input_y  = touch_data[3] | (touch_data[4] << 8);

            input_mt_slot(dev->input, id);
		    input_mt_report_slot_state(dev->input, MT_TOOL_FINGER, true);
		    input_report_abs(dev->input, ABS_MT_POSITION_X, input_x);
		    input_report_abs(dev->input, ABS_MT_POSITION_Y, input_y);
        }
    } else if(touch_num == 0){                /* 单点触摸释放 */
        input_mt_slot(dev->input, id);
        input_mt_report_slot_state(dev->input, MT_TOOL_FINGER, false);
    }

	input_mt_report_pointer_emulation(dev->input, true);
    input_sync(dev->input);

    data = 0x00;                /* 向0X814E寄存器写0 */
    gt9147_write_regs(dev, GT_GSTID_REG, &data, 1);

fail:
	return IRQ_HANDLED;
}


/*
 * @description     : GT9147中断初始化
 * @param - client 	: 要操作的i2c
 * @param - multidev: 自定义的multitouch设备
 * @return          : 0，成功;其他负值,失败
 */
static int gt9147_ts_irq(struct i2c_client *client, struct gt9147_dev *dev)
{
	int ret = 0;
	/* 2，申请中断,client->irq就是IO中断， */
	ret = devm_request_threaded_irq(&client->dev, client->irq, NULL,
					gt9147_irq_handler, irq_table[dev->irqtype] | IRQF_ONESHOT,
					client->name, &gt9147);
	if (ret) {
		dev_err(&client->dev, "Unable to request touchscreen IRQ.\n");
		return ret;
	}

	return 0;
}

/*
 * @description     : GT9147读取固件
 * @param - client 	: 要操作的i2c
 * @param - multidev: 自定义的multitouch设备
 * @return          : 0，成功;其他负值,失败
 */
static int gt9147_read_firmware(struct i2c_client *client, struct gt9147_dev *dev)
{
	int ret = 0, version = 0;
	u16 id = 0;
	u8 data[7]={0};
	char id_str[5];
	ret = gt9147_read_regs(dev, GT_PID_REG, data, 6);
	if (ret) {
		dev_err(&client->dev, "Unable to read PID.\n");
		return ret;
	}
	memcpy(id_str, data, 4);
	id_str[4] = 0;
    if (kstrtou16(id_str, 10, &id))
        id = 0x1001;
	version = get_unaligned_le16(&data[4]);
	dev_info(&client->dev, "ID %d, version: %04x\n", id, version);
	switch (id) {    /* 由于不同的芯片配置寄存器地址不一样需要判断一下  */
    case 1151:
    case 1158:
    case 5663:
    case 5688:    /* 读取固件里面的配置信息  */
        ret = gt9147_read_regs(dev, GT_1xx_CFGS_REG, data, 7);  
		break;
    default:
        ret = gt9147_read_regs(dev, GT_9xx_CFGS_REG, data, 7);
		break;
    }
	if (ret) {
		dev_err(&client->dev, "Unable to read Firmware.\n");
		return ret;
	}
	dev->max_x = (data[2] << 8) + data[1];
	dev->max_y = (data[4] << 8) + data[3];
	dev->irqtype = data[6] & 0x3;
	printk("X_MAX: %d, Y_MAX: %d, TRIGGER: 0x%02x", dev->max_x, dev->max_y, dev->irqtype);

	return 0;
}

int gt9147_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    u8 data, ret;
    gt9147.client = client;

 	/* 1，获取设备树中的中断和复位引脚 */
	gt9147.irq_pin = of_get_named_gpio(client->dev.of_node, "interrupt-gpios", 0);
	gt9147.reset_pin = of_get_named_gpio(client->dev.of_node, "reset-gpios", 0);

	/* 2，复位GT9147 */
	ret = gt9147_ts_reset(client, &gt9147);
	if(ret < 0) {
		goto fail;
    }

    /* 3，初始化GT9147 */
    data = 0x02;
    gt9147_write_regs(&gt9147, GT_CTRL_REG, &data, 1); /* 软复位 */
    mdelay(100);
    data = 0x0;
    gt9147_write_regs(&gt9147, GT_CTRL_REG, &data, 1); /* 停止软复位 */
    mdelay(100);
	
    
    /* 4,初始化GT9147，读取固件  */
	ret = gt9147_read_firmware(client, &gt9147);
	if(ret != 0) {
		printk("Fail !!! check !!\r\n");
		goto fail;
    }
	
    /* 5，input设备注册 */
	gt9147.input = devm_input_allocate_device(&client->dev);
	if (!gt9147.input) {
		ret = -ENOMEM;
		goto fail;
	}
	gt9147.input->name = client->name;
	gt9147.input->id.bustype = BUS_I2C;
	gt9147.input->dev.parent = &client->dev;

	__set_bit(EV_KEY, gt9147.input->evbit);
	__set_bit(EV_ABS, gt9147.input->evbit);
	__set_bit(BTN_TOUCH, gt9147.input->keybit);

	input_set_abs_params(gt9147.input, ABS_X, 0, gt9147.max_x, 0, 0);
	input_set_abs_params(gt9147.input, ABS_Y, 0, gt9147.max_y, 0, 0);
	input_set_abs_params(gt9147.input, ABS_MT_POSITION_X,0, gt9147.max_x, 0, 0);
	input_set_abs_params(gt9147.input, ABS_MT_POSITION_Y,0, gt9147.max_y, 0, 0);	     
	ret = input_mt_init_slots(gt9147.input, MAX_SUPPORT_POINTS, 0);
	if (ret) {
		goto fail;
	}

	ret = input_register_device(gt9147.input);
	if (ret)
		goto fail;

    /* 6，最后初始化中断 */
	ret = gt9147_ts_irq(client, &gt9147);
	if(ret < 0) {
		goto fail;
	}
    return 0;

fail:
	return ret;
}

/*
 * @description     : i2c驱动的remove函数，移除i2c驱动的时候此函数会执行
 * @param - client 	: i2c设备
 * @return          : 0，成功;其他负值,失败
 */
int gt9147_remove(struct i2c_client *client)
{
    input_unregister_device(gt9147.input);
    return 0;
}

/*
 *  传统驱动匹配表
 */ 
const struct i2c_device_id gt9147_id_table[] = {
	{ "goodix,gt9147", 0, },
    { /* sentinel */ }
};

/*
 * 设备树匹配表 
 */
const struct of_device_id gt9147_of_match_table[] = {
    {.compatible = "goodix,gt9147" },
    { /* sentinel */ }
};

/* i2c驱动结构体 */	
struct i2c_driver gt9147_i2c_driver = {
    .driver = {
        .name  = "gt9147",
        .owner = THIS_MODULE,
        .of_match_table = gt9147_of_match_table,
    },
    .id_table = gt9147_id_table,
    .probe  = gt9147_probe,
    .remove = gt9147_remove,
};

module_i2c_driver(gt9147_i2c_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zuozhongkai");


