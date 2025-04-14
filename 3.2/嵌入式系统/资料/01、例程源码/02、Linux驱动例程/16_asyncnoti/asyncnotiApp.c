#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include "poll.h"
#include "sys/select.h"
#include "sys/time.h"
#include "linux/ioctl.h"
#include "signal.h"
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: asyncnotiApp.c
作者	  	: 左忠凯
版本	   	: V1.0
描述	   	: 异步通知测试APP
其他	   	: 无
使用方法	：./asyncnotiApp /dev/asyncnoti 打开测试App
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/8/13 左忠凯创建
***************************************************************/

static int fd = 0;	/* 文件描述符 */

/*
 * SIGIO信号处理函数
 * @param - signum 	: 信号值
 * @return 			: 无
 */
static void sigio_signal_func(int signum)
{
	int err = 0;
	unsigned int keyvalue = 0;

	err = read(fd, &keyvalue, sizeof(keyvalue));
	if(err < 0) {
		/* 读取错误 */
	} else {
		printf("sigio signal! key value=%d\r\n", keyvalue);
	}
}

/*
 * @description		: main主程序
 * @param - argc 	: argv数组元素个数
 * @param - argv 	: 具体参数
 * @return 			: 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
	int flags = 0;
	char *filename;

	if (argc != 2) {
		printf("Error Usage!\r\n");
		return -1;
	}

	filename = argv[1];
	fd = open(filename, O_RDWR);
	if (fd < 0) {
		printf("Can't open file %s\r\n", filename);
		return -1;
	}

	/* 设置信号SIGIO的处理函数 */
	signal(SIGIO, sigio_signal_func);
	
	fcntl(fd, F_SETOWN, getpid());		/* 设置当前进程接收SIGIO信号 	*/
	flags = fcntl(fd, F_GETFL);			/* 获取当前的进程状态 			*/
	fcntl(fd, F_SETFL, flags | FASYNC);	/* 设置进程启用异步通知功能 	*/	

	while(1) {
		sleep(2);
	}

	close(fd);
	return 0;
}
