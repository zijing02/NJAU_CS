#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: miscbeepApp.c
作者	  	: 左忠凯
版本	   	: V1.0
描述	   	: MISC驱动框架下的beep测试APP。
其他	   	: 无
使用方法	 ：./miscbeepApp  /dev/miscbeep  0 关闭蜂鸣器
		     ./misdcbeepApp /dev/miscbeep  1 打开蜂鸣器
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/8/20 左忠凯创建
***************************************************************/
#define BEEPOFF	0
#define BEEPON 	1

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
	unsigned char databuf[1];
	
	if(argc != 3){
		printf("Error Usage!\r\n");
		return -1;
	}

	filename = argv[1];
	fd = open(filename, O_RDWR);	/* 打开beep驱动 */
	if(fd < 0){
		printf("file %s open failed!\r\n", argv[1]);
		return -1;
	}

	databuf[0] = atoi(argv[2]);	/* 要执行的操作：打开或关闭 */
	retvalue = write(fd, databuf, sizeof(databuf));
	if(retvalue < 0){
		printf("BEEP Control Failed!\r\n");
		close(fd);
		return -1;
	}

	retvalue = close(fd); /* 关闭文件 */
	if(retvalue < 0){
		printf("file %s close failed!\r\n", argv[1]);
		return -1;
	}
	return 0;
}
