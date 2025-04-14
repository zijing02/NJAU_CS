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
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>

#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: ramdisk.c
作者	  	: 左忠凯
版本	   	: V1.0
描述	   	: 内存模拟硬盘，实现块设备驱动，本驱动使用请求队列。
其他	   	: 无
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2020/5/22 左忠凯创建
***************************************************************/

#define RAMDISK_SIZE	(2 * 1024 * 1024) 	/* 容量大小为2MB */
#define RAMDISK_NAME	"ramdisk"			/* 名字 */
#define RADMISK_MINOR	3					/* 表示有三个磁盘分区！不是次设备号为3！ */

/* ramdisk设备结构体 */
struct ramdisk_dev{
	int major;					/* 主设备号 */
	unsigned char *ramdiskbuf;	/* ramdisk内存空间,用于模拟块设备 */
	spinlock_t lock;			/* 自旋锁 */
	struct gendisk *gendisk; 	/* gendisk */
	struct request_queue *queue;/* 请求队列 */

};

struct ramdisk_dev ramdisk;		/* ramdisk设备 */

/*
 * @description		: 打开块设备
 * @param - dev 	: 块设备
 * @param - mode 	: 打开模式
 * @return 			: 0 成功;其他 失败
 */
int ramdisk_open(struct block_device *dev, fmode_t mode)
{
	printk("ramdisk open\r\n");
	return 0;
}

/*
 * @description		: 释放块设备
 * @param - disk 	: gendisk
 * @param - mode 	: 模式
 * @return 			: 0 成功;其他 失败
 */
void ramdisk_release(struct gendisk *disk, fmode_t mode)
{
	printk("ramdisk release\r\n");
}

/*
 * @description		: 获取磁盘信息
 * @param - dev 	: 块设备
 * @param - geo 	: 模式
 * @return 			: 0 成功;其他 失败
 */
int ramdisk_getgeo(struct block_device *dev, struct hd_geometry *geo)
{
	/* 这是相对于机械硬盘的概念 */
	geo->heads = 2;			/* 磁头 */
	geo->cylinders = 32;	/* 柱面 */
	geo->sectors = RAMDISK_SIZE / (2 * 32 *512); /* 一个磁道上的扇区数量 */
	return 0;
}

/* 
 * 块设备操作函数 
 */
static struct block_device_operations ramdisk_fops =
{
	.owner	 = THIS_MODULE,
	.open	 = ramdisk_open,
	.release = ramdisk_release,
	.getgeo  = ramdisk_getgeo,
};

/*
 * @description	: 处理传输过程
 * @param-req 	: 请求
 * @return 		: 无
 */
static void ramdisk_transfer(struct request *req)
{	
	unsigned long start = blk_rq_pos(req) << 9;  	/* blk_rq_pos获取到的是扇区地址，左移9位转换为字节地址 */
	unsigned long len  = blk_rq_cur_bytes(req);		/* 大小   */

	/* bio中的数据缓冲区
	 * 读：从磁盘读取到的数据存放到buffer中
	 * 写：buffer保存这要写入磁盘的数据
	 */
	void *buffer = bio_data(req->bio);		
	
	if(rq_data_dir(req) == READ) 		/* 读数据 */	
		memcpy(buffer, ramdisk.ramdiskbuf + start, len);
	else if(rq_data_dir(req) == WRITE) 	/* 写数据 */
		memcpy(ramdisk.ramdiskbuf + start, buffer, len);

}

/*
 * @description	: 请求处理函数
 * @param-q 	: 请求队列
 * @return 		: 无
 */
void ramdisk_request_fn(struct request_queue *q)
{
	int err = 0;
	struct request *req;

	/* 循环处理请求队列中的每个请求 */
	req = blk_fetch_request(q);
	while(req != NULL) {

		/* 针对请求做具体的传输处理 */
		ramdisk_transfer(req);

		/* 判断是否为最后一个请求，如果不是的话就获取下一个请求
		 * 循环处理完请求队列中的所有请求。
		 */
		if (!__blk_end_request_cur(req, err))
			req = blk_fetch_request(q);
	}
}


/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static int __init ramdisk_init(void)
{
	int ret = 0;
	printk("ramdisk init\r\n");

	/* 1、申请用于ramdisk内存 */
	ramdisk.ramdiskbuf = kzalloc(RAMDISK_SIZE, GFP_KERNEL);
	if(ramdisk.ramdiskbuf == NULL) {
		ret = -EINVAL;
		goto ram_fail;
	}

	/* 2、初始化自旋锁 */
	spin_lock_init(&ramdisk.lock);

	/* 3、注册块设备 */
	ramdisk.major = register_blkdev(0, RAMDISK_NAME); /* 由系统自动分配主设备号 */
	if(ramdisk.major < 0) {
		goto register_blkdev_fail;
	}  
	printk("ramdisk major = %d\r\n", ramdisk.major);

	/* 4、分配并初始化gendisk */
	ramdisk.gendisk = alloc_disk(RADMISK_MINOR);
	if(!ramdisk.gendisk) {
		ret = -EINVAL;
		goto gendisk_alloc_fail;
	}

	/* 5、分配并初始化请求队列 */
	ramdisk.queue = blk_init_queue(ramdisk_request_fn, &ramdisk.lock);
	if(!ramdisk.queue) {
		ret = EINVAL;
		goto blk_init_fail;
	}

	/* 6、添加(注册)disk */
	ramdisk.gendisk->major = ramdisk.major;		/* 主设备号 */
	ramdisk.gendisk->first_minor = 0;			/* 第一个次设备号(起始次设备号) */
	ramdisk.gendisk->fops = &ramdisk_fops; 		/* 操作函数 */
	ramdisk.gendisk->private_data = &ramdisk;	/* 私有数据 */
	ramdisk.gendisk->queue = ramdisk.queue;		/* 请求队列 */
	sprintf(ramdisk.gendisk->disk_name, RAMDISK_NAME); /* 名字 */
	set_capacity(ramdisk.gendisk, RAMDISK_SIZE/512);	/* 设备容量(单位为扇区) */
	add_disk(ramdisk.gendisk);

	return 0;

blk_init_fail:
	put_disk(ramdisk.gendisk);
	//del_gendisk(ramdisk.gendisk);
gendisk_alloc_fail:
	unregister_blkdev(ramdisk.major, RAMDISK_NAME);
register_blkdev_fail:
	kfree(ramdisk.ramdiskbuf); /* 释放内存 */
ram_fail:
	return ret;
}

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static void __exit ramdisk_exit(void)
{
	printk("ramdisk exit\r\n");
	/* 释放gendisk */
	del_gendisk(ramdisk.gendisk);
	put_disk(ramdisk.gendisk);

	/* 清除请求队列 */
	blk_cleanup_queue(ramdisk.queue);

	/* 注销块设备 */
	unregister_blkdev(ramdisk.major, RAMDISK_NAME);

	/* 释放内存 */
	kfree(ramdisk.ramdiskbuf); 
}

module_init(ramdisk_init);
module_exit(ramdisk_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zuozhongkai");

