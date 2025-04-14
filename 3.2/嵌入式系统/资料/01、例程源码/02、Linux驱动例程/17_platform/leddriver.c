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
#include <linux/irq.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/platform_device.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: leddriver.c
作者	  	: 左忠凯
版本	   	: V1.0
描述	   	: platform驱动
其他	   	: 无
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/8/13 左忠凯创建
***************************************************************/

#define LEDDEV_CNT		1			/* 设备号长度 	*/
#define LEDDEV_NAME		"platled"	/* 设备名字 	*/
#define LEDOFF 			0
#define LEDON 			1

/* 寄存器名 */
static void __iomem *IMX6U_CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO03;
static void __iomem *SW_PAD_GPIO1_IO03;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;

/* leddev设备结构体 */
struct leddev_dev{
	dev_t devid;			/* 设备号	*/
	struct cdev cdev;		/* cdev		*/
	struct class *class;	/* 类 		*/
	struct device *device;	/* 设备		*/
	int major;				/* 主设备号	*/		
};

struct leddev_dev leddev; 	/* led设备 */

/*
 * @description		: LED打开/关闭
 * @param - sta 	: LEDON(0) 打开LED，LEDOFF(1) 关闭LED
 * @return 			: 无
 */
void led0_switch(u8 sta)
{
	u32 val = 0;
	if(sta == LEDON){
		val = readl(GPIO1_DR);
		val &= ~(1 << 3);	
		writel(val, GPIO1_DR);
	}else if(sta == LEDOFF){
		val = readl(GPIO1_DR);
		val|= (1 << 3);	
		writel(val, GPIO1_DR);
	}	
}

/*
 * @description		: 打开设备
 * @param - inode 	: 传递给驱动的inode
 * @param - filp 	: 设备文件，file结构体有个叫做private_data的成员变量
 * 					  一般在open的时候将private_data指向设备结构体。
 * @return 			: 0 成功;其他 失败
 */
static int led_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &leddev; /* 设置私有数据  */
	return 0;
}

/*
 * @description		: 向设备写数据 
 * @param - filp 	: 设备文件，表示打开的文件描述符
 * @param - buf 	: 要写给设备写入的数据
 * @param - cnt 	: 要写入的数据长度
 * @param - offt 	: 相对于文件首地址的偏移
 * @return 			: 写入的字节数，如果为负值，表示写入失败
 */
static ssize_t led_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
	int retvalue;
	unsigned char databuf[1];
	unsigned char ledstat;

	retvalue = copy_from_user(databuf, buf, cnt);
	if(retvalue < 0) {
		return -EFAULT;
	}

	ledstat = databuf[0];		/* 获取状态值 */
	if(ledstat == LEDON) {
		led0_switch(LEDON);		/* 打开LED灯 */
	}else if(ledstat == LEDOFF) {
		led0_switch(LEDOFF);	/* 关闭LED灯 */
	}
	return 0;
}

/* 设备操作函数 */
static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.write = led_write,
};

/*
 * @description		: flatform驱动的probe函数，当驱动与
 * 					  设备匹配以后此函数就会执行
 * @param - dev 	: platform设备
 * @return 			: 0，成功;其他负值,失败
 */
static int led_probe(struct platform_device *dev)
{	
	int i = 0;
	int ressize[5];
	u32 val = 0;
	struct resource *ledsource[5];

	printk("led driver and device has matched!\r\n");
	/* 1、获取资源 */
	for (i = 0; i < 5; i++) {
		ledsource[i] = platform_get_resource(dev, IORESOURCE_MEM, i); /* 依次MEM类型资源 */
		if (!ledsource[i]) {
			dev_err(&dev->dev, "No MEM resource for always on\n");
			return -ENXIO;
		}
		ressize[i] = resource_size(ledsource[i]);	
	}	

	/* 2、初始化LED */
	/* 寄存器地址映射 */
 	IMX6U_CCM_CCGR1 = ioremap(ledsource[0]->start, ressize[0]);
	SW_MUX_GPIO1_IO03 = ioremap(ledsource[1]->start, ressize[1]);
  	SW_PAD_GPIO1_IO03 = ioremap(ledsource[2]->start, ressize[2]);
	GPIO1_DR = ioremap(ledsource[3]->start, ressize[3]);
	GPIO1_GDIR = ioremap(ledsource[4]->start, ressize[4]);
	
	val = readl(IMX6U_CCM_CCGR1);
	val &= ~(3 << 26);				/* 清除以前的设置 */
	val |= (3 << 26);				/* 设置新值 */
	writel(val, IMX6U_CCM_CCGR1);

	/* 设置GPIO1_IO03复用功能，将其复用为GPIO1_IO03 */
	writel(5, SW_MUX_GPIO1_IO03);
	writel(0x10B0, SW_PAD_GPIO1_IO03);

	/* 设置GPIO1_IO03为输出功能 */
	val = readl(GPIO1_GDIR);
	val &= ~(1 << 3);			/* 清除以前的设置 */
	val |= (1 << 3);			/* 设置为输出 */
	writel(val, GPIO1_GDIR);

	/* 默认关闭LED1 */
	val = readl(GPIO1_DR);
	val |= (1 << 3) ;	
	writel(val, GPIO1_DR);
	
	/* 注册字符设备驱动 */
	/*1、创建设备号 */
	if (leddev.major) {		/*  定义了设备号 */
		leddev.devid = MKDEV(leddev.major, 0);
		register_chrdev_region(leddev.devid, LEDDEV_CNT, LEDDEV_NAME);
	} else {						/* 没有定义设备号 */
		alloc_chrdev_region(&leddev.devid, 0, LEDDEV_CNT, LEDDEV_NAME);	/* 申请设备号 */
		leddev.major = MAJOR(leddev.devid);	/* 获取分配号的主设备号 */
	}
	
	/* 2、初始化cdev */
	leddev.cdev.owner = THIS_MODULE;
	cdev_init(&leddev.cdev, &led_fops);
	
	/* 3、添加一个cdev */
	cdev_add(&leddev.cdev, leddev.devid, LEDDEV_CNT);

	/* 4、创建类 */
	leddev.class = class_create(THIS_MODULE, LEDDEV_NAME);
	if (IS_ERR(leddev.class)) {
		return PTR_ERR(leddev.class);
	}

	/* 5、创建设备 */
	leddev.device = device_create(leddev.class, NULL, leddev.devid, NULL, LEDDEV_NAME);
	if (IS_ERR(leddev.device)) {
		return PTR_ERR(leddev.device);
	}

	return 0;
}

/*
 * @description		: platform驱动的remove函数，移除platform驱动的时候此函数会执行
 * @param - dev 	: platform设备
 * @return 			: 0，成功;其他负值,失败
 */
static int led_remove(struct platform_device *dev)
{
	iounmap(IMX6U_CCM_CCGR1);
	iounmap(SW_MUX_GPIO1_IO03);
	iounmap(SW_PAD_GPIO1_IO03);
	iounmap(GPIO1_DR);
	iounmap(GPIO1_GDIR);

	cdev_del(&leddev.cdev);/*  删除cdev */
	unregister_chrdev_region(leddev.devid, LEDDEV_CNT); /* 注销设备号 */
	device_destroy(leddev.class, leddev.devid);
	class_destroy(leddev.class);
	return 0;
}

/* platform驱动结构体 */
static struct platform_driver led_driver = {
	.driver		= {
		.name	= "imx6ul-led",			/* 驱动名字，用于和设备匹配 */
	},
	.probe		= led_probe,
	.remove		= led_remove,
};
		
/*
 * @description	: 驱动模块加载函数
 * @param 		: 无
 * @return 		: 无
 */
static int __init leddriver_init(void)
{
	return platform_driver_register(&led_driver);
}

/*
 * @description	: 驱动模块卸载函数
 * @param 		: 无
 * @return 		: 无
 */
static void __exit leddriver_exit(void)
{
	platform_driver_unregister(&led_driver);
}

module_init(leddriver_init);
module_exit(leddriver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zuozhongkai");



