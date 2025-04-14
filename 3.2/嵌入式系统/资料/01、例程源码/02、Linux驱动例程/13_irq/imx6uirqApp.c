#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include "linux/ioctl.h"
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: imx6uirqApp.c
作者	  	: 左忠凯
版本	   	: V1.0
描述	   	: 定时器测试应用程序
其他	   	: 无
使用方法	：./imx6uirqApp /dev/imx6uirq 打开测试App
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/7/26 左忠凯创建
***************************************************************/

/*
 * @description		: main主程序
 * @param - argc 	: argv数组元素个数
 * @param - argv 	: 具体参数
 * @return 			: 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
	int fd;
	int ret = 0;
	char *filename;
	unsigned char data;
	
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

	while (1) {
		ret = read(fd, &data, sizeof(data));
		if (ret < 0) {  /* 数据读取错误或者无效 */
			
		} else {		/* 数据读取正确 */
			if (data)	/* 读取到数据 */
				printf("key value = %#X\r\n", data);
		}
	}
	close(fd);
	return ret;
}
