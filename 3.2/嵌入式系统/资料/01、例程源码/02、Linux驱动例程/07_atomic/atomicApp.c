#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: atomicApp.c
作者	  	: 左忠凯
版本	   	: V1.0
描述	   	: 原子变量测试APP，测试原子变量能不能实现一次
			 只允许一个应用程序使用LED。
其他	   	: 无
使用方法	 ：./atomicApp /dev/gpioled  0 关闭LED灯
		     ./atomicApp /dev/gpioled  1 打开LED灯
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/1/30 左忠凯创建
***************************************************************/

#define LEDOFF 	0
#define LEDON 	1

/*
 * @description		: main主程序
 * @param - argc 	: argv数组元素个数
 * @param - argv 	: 具体参数
 * @return 			: 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
	int fd, retvalue;
	char *filename;
	unsigned char cnt = 0;
	unsigned char databuf[1];
	
	if(argc != 3){
		printf("Error Usage!\r\n");
		return -1;
	}

	filename = argv[1];

	/* 打开beep驱动 */
	fd = open(filename, O_RDWR);
	if(fd < 0){
		printf("file %s open failed!\r\n", argv[1]);
		return -1;
	}

	databuf[0] = atoi(argv[2]);	/* 要执行的操作：打开或关闭 */

	/* 向/dev/gpioled文件写入数据 */
	retvalue = write(fd, databuf, sizeof(databuf));
	if(retvalue < 0){
		printf("LED Control Failed!\r\n");
		close(fd);
		return -1;
	}

	/* 模拟占用25S LED */
	while(1) {
		sleep(5);
		cnt++;
		printf("App running times:%d\r\n", cnt);
		if(cnt >= 5) break;
	}

	printf("App running finished!");
	retvalue = close(fd); /* 关闭文件 */
	if(retvalue < 0){
		printf("file %s close failed!\r\n", argv[1]);
		return -1;
	}
	return 0;
}
