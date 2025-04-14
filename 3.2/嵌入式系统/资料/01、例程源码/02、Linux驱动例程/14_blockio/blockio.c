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
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/of_irq.h>
#include <linux/irq.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: block.c
作者	  	: 左忠凯
版本	   	: V1.0
描述	   	: 阻塞IO访问
其他	   	: 无
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/7/26 左忠凯创建
***************************************************************/
#define IMX6UIRQ_CNT		1			/* 设备号个数 	*/
#define IMX6UIRQ_NAME		"blockio"	/* 名字 		*/
#define KEY0VALUE			0X01		/* KEY0按键值 	*/
#define INVAKEY				0XFF		/* 无效的按键值 */
#define KEY_NUM				1			/* 按键数量 	*/

/* 中断IO描述结构体 */
struct irq_keydesc {
	int gpio;								/* gpio */
	int irqnum;								/* 中断号     */
	unsigned char value;					/* 按键对应的键值 */
	char name[10];							/* 名字 */
	irqreturn_t (*handler)(int, void *);	/* 中断服务函数 */
};

/* imx6uirq设备结构体 */
struct imx6uirq_dev{
	dev_t devid;			/* 设备号 	 */	
	struct cdev cdev;		/* cdev 	*/                 
	struct class *class;	/* 类 		*/
	struct device *device;	/* 设备 	 */
	int major;				/* 主设备号	  */
	int minor;				/* 次设备号   */
	struct device_node	*nd; /* 设备节点 */	
	atomic_t keyvalue;		/* 有效的按键键值 */
	atomic_t releasekey;	/* 标记是否完成一次完成的按键，包括按下和释放 */
	struct timer_list timer;/* 定义一个定时器*/
	struct irq_keydesc irqkeydesc[KEY_NUM];	/* 按键init述数组 */
	unsigned char curkeynum;				/* 当前init按键号 */

	wait_queue_head_t r_wait;	/* 读等待队列头 */
};

struct imx6uirq_dev imx6uirq;	/* irq设备 */

/* @description		: 中断服务函数，开启定时器		
 *				  	  定时器用于按键消抖。
 * @param - irq 	: 中断号 
 * @param - dev_id	: 设备结构。
 * @return 			: 中断执行结果
 */
static irqreturn_t key0_handler(int irq, void *dev_id)
{
	struct imx6uirq_dev *dev = (struct imx6uirq_dev*)dev_id;

	dev->curkeynum = 0;
	dev->timer.data = (volatile long)dev_id;
	mod_timer(&dev->timer, jiffies + msecs_to_jiffies(10));	/* 10ms定时 */
	return IRQ_RETVAL(IRQ_HANDLED);
}

/* @description	: 定时器服务函数，用于按键消抖，定时器到了以后
 *				  再次读取按键值，如果按键还是处于按下状态就表示按键有效。
 * @param - arg	: 设备结构变量
 * @return 		: 无
 */
void timer_function(unsigned long arg)
{
	unsigned char value;
	unsigned char num;
	struct irq_keydesc *keydesc;
	struct imx6uirq_dev *dev = (struct imx6uirq_dev *)arg;

	num = dev->curkeynum;
	keydesc = &dev->irqkeydesc[num];

	value = gpio_get_value(keydesc->gpio); 	/* 读取IO值 */
	if(value == 0){ 						/* 按下按键 */
		atomic_set(&dev->keyvalue, keydesc->value);
	}
	else{ 									/* 按键松开 */
		atomic_set(&dev->keyvalue, 0x80 | keydesc->value);
		atomic_set(&dev->releasekey, 1);	/* 标记松开按键，即完成一次完整的按键过程 */
	}               

	/* 唤醒进程 */
	if(atomic_read(&dev->releasekey)) {	/* 完成一次按键过程 */
		/* wake_up(&dev->r_wait); */
		wake_up_interruptible(&dev->r_wait);
	}
}

/*
 * @description	: 按键IO初始化
 * @param 		: 无
 * @return 		: 无
 */
static int keyio_init(void)
{
	unsigned char i = 0;
	char name[10];
	int ret = 0;
	
	imx6uirq.nd = of_find_node_by_path("/key");
	if (imx6uirq.nd== NULL){
		printk("key node not find!\r\n");
		return -EINVAL;
	} 

	/* 提取GPIO */
	for (i = 0; i < KEY_NUM; i++) {
		imx6uirq.irqkeydesc[i].gpio = of_get_named_gpio(imx6uirq.nd ,"key-gpio", i);
		if (imx6uirq.irqkeydesc[i].gpio < 0) {
			printk("can't get key%d\r\n", i);
		}
	}
	
	/* 初始化key所使用的IO，并且设置成中断模式 */
	for (i = 0; i < KEY_NUM; i++) {
		memset(imx6uirq.irqkeydesc[i].name, 0, sizeof(name));	/* 缓冲区清零 */
		sprintf(imx6uirq.irqkeydesc[i].name, "KEY%d", i);		/* 组合名字 */
		gpio_request(imx6uirq.irqkeydesc[i].gpio, name);
		gpio_direction_input(imx6uirq.irqkeydesc[i].gpio);	
		imx6uirq.irqkeydesc[i].irqnum = irq_of_parse_and_map(imx6uirq.nd, i);
#if 0
		imx6uirq.irqkeydesc[i].irqnum = gpio_to_irq(imx6uirq.irqkeydesc[i].gpio);
#endif
	}

	/* 申请中断 */
	imx6uirq.irqkeydesc[0].handler = key0_handler;
	imx6uirq.irqkeydesc[0].value = KEY0VALUE;
	
	for (i = 0; i < KEY_NUM; i++) {
		ret = request_irq(imx6uirq.irqkeydesc[i].irqnum, imx6uirq.irqkeydesc[i].handler, 
		                 IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING, imx6uirq.irqkeydesc[i].name, &imx6uirq);
		if(ret < 0){
			printk("irq %d request failed!\r\n", imx6uirq.irqkeydesc[i].irqnum);
			return -EFAULT;
		}
	}

	/* 创建定时器 */
     init_timer(&imx6uirq.timer);
     imx6uirq.timer.function = timer_function;

	/* 初始化等待队列头 */
	init_waitqueue_head(&imx6uirq.r_wait);
	return 0;
}

/*
 * @description		: 打开设备
 * @param - inode 	: 传递给驱动的inode
 * @param - filp 	: 设备文件，file结构体有个叫做private_data的成员变量
 * 					  一般在open的时候将private_data指向设备结构体。
 * @return 			: 0 成功;其他 失败
 */
static int imx6uirq_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &imx6uirq;	/* 设置私有数据 */
	return 0;
}

 /*
  * @description     : 从设备读取数据 
  * @param - filp    : 要打开的设备文件(文件描述符)
  * @param - buf     : 返回给用户空间的数据缓冲区
  * @param - cnt     : 要读取的数据长度
  * @param - offt    : 相对于文件首地址的偏移
  * @return          : 读取的字节数，如果为负值，表示读取失败
  */
static ssize_t imx6uirq_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	int ret = 0;
	unsigned char keyvalue = 0;
	unsigned char releasekey = 0;
	struct imx6uirq_dev *dev = (struct imx6uirq_dev *)filp->private_data;

#if 0
	/* 加入等待队列，等待被唤醒,也就是有按键按下 */
 	ret = wait_event_interruptible(dev->r_wait, atomic_read(&dev->releasekey)); 
	if (ret) {
		goto wait_error;
	} 
#endif

	DECLARE_WAITQUEUE(wait, current);	/* 定义一个等待队列 */
	if(atomic_read(&dev->releasekey) == 0) {	/* 没有按键按下 */
		add_wait_queue(&dev->r_wait, &wait);	/* 将等待队列添加到等待队列头 */
		__set_current_state(TASK_INTERRUPTIBLE);/* 设置任务状态 */
		schedule();							/* 进行一次任务切换 */
		if(signal_pending(current))	{			/* 判断是否为信号引起的唤醒 */
			ret = -ERESTARTSYS;
			goto wait_error;
		}
		__set_current_state(TASK_RUNNING);      /* 将当前任务设置为运行状态 */
	    remove_wait_queue(&dev->r_wait, &wait);    /* 将对应的队列项从等待队列头删除 */
	}

	keyvalue = atomic_read(&dev->keyvalue);
	releasekey = atomic_read(&dev->releasekey);

	if (releasekey) { /* 有按键按下 */	
		if (keyvalue & 0x80) {
			keyvalue &= ~0x80;
			ret = copy_to_user(buf, &keyvalue, sizeof(keyvalue));
		} else {
			goto data_error;
		}
		atomic_set(&dev->releasekey, 0);/* 按下标志清零 */
	} else {
		goto data_error;
	}
	return 0;

wait_error:
	set_current_state(TASK_RUNNING);		/* 设置任务为运行态 */
	remove_wait_queue(&dev->r_wait, &wait);	/* 将等待队列移除 */
	return ret;

data_error:
	return -EINVAL;
}

/* 设备操作函数 */
static struct file_operations imx6uirq_fops = {
	.owner = THIS_MODULE,
	.open = imx6uirq_open,
	.read = imx6uirq_read,
};

/*
 * @description	: 驱动入口函数
 * @param 		: 无
 * @return 		: 无
 */
static int __init imx6uirq_init(void)
{
	/* 1、构建设备号 */
	if (imx6uirq.major) {
		imx6uirq.devid = MKDEV(imx6uirq.major, 0);
		register_chrdev_region(imx6uirq.devid, IMX6UIRQ_CNT, IMX6UIRQ_NAME);
	} else {
		alloc_chrdev_region(&imx6uirq.devid, 0, IMX6UIRQ_CNT, IMX6UIRQ_NAME);
		imx6uirq.major = MAJOR(imx6uirq.devid);
		imx6uirq.minor = MINOR(imx6uirq.devid);
	}

	/* 2、注册字符设备 */
	cdev_init(&imx6uirq.cdev, &imx6uirq_fops);
	cdev_add(&imx6uirq.cdev, imx6uirq.devid, IMX6UIRQ_CNT);

	/* 3、创建类 */
	imx6uirq.class = class_create(THIS_MODULE, IMX6UIRQ_NAME);
	if (IS_ERR(imx6uirq.class)) {	
		return PTR_ERR(imx6uirq.class);
	}

	/* 4、创建设备 */
	imx6uirq.device = device_create(imx6uirq.class, NULL, imx6uirq.devid, NULL, IMX6UIRQ_NAME);
	if (IS_ERR(imx6uirq.device)) {
		return PTR_ERR(imx6uirq.device);
	}
		
	/* 5、始化按键 */
	atomic_set(&imx6uirq.keyvalue, INVAKEY);
	atomic_set(&imx6uirq.releasekey, 0);
	keyio_init();
	return 0;
}

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static void __exit imx6uirq_exit(void)
{
	unsigned i = 0;
	/* 删除定时器 */
	del_timer_sync(&imx6uirq.timer);	/* 删除定时器 */
		
	/* 释放中断 */	
	for (i = 0; i < KEY_NUM; i++) {
		free_irq(imx6uirq.irqkeydesc[i].irqnum, &imx6uirq);
		gpio_free(imx6uirq.irqkeydesc[i].gpio);
	}
	cdev_del(&imx6uirq.cdev);
	unregister_chrdev_region(imx6uirq.devid, IMX6UIRQ_CNT);
	device_destroy(imx6uirq.class, imx6uirq.devid);
	class_destroy(imx6uirq.class);
}
	
module_init(imx6uirq_init);
module_exit(imx6uirq_exit);
MODULE_LICENSE("GPL");
	
	