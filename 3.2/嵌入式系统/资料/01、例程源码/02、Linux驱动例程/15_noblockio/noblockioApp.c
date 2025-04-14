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
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: noblockApp.c
作者	  	: 左忠凯
版本	   	: V1.0
描述	   	: 非阻塞访问测试APP
其他	   	: 无
使用方法	：./blockApp /dev/blockio 打开测试App
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/9/8 左忠凯创建
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
	struct pollfd fds;
	fd_set readfds;
	struct timeval timeout;
	unsigned char data;

	if (argc != 2) {
		printf("Error Usage!\r\n");
		return -1;
	}

	filename = argv[1];
	fd = open(filename, O_RDWR | O_NONBLOCK);	/* 非阻塞访问 */
	if (fd < 0) {
		printf("Can't open file %s\r\n", filename);
		return -1;
	}

#if 0
	/* 构造结构体 */
	fds.fd = fd;
	fds.events = POLLIN;
		
	while (1) {
		ret = poll(&fds, 1, 500);
		if (ret) {	/* 数据有效 */
			ret = read(fd, &data, sizeof(data));
			if(ret < 0) {
				/* 读取错误 */
			} else {
				if(data)
					printf("key value = %d \r\n", data);
			} 	
		} else if (ret == 0) { 	/* 超时 */
			/* 用户自定义超时处理 */
		} else if (ret < 0) {	/* 错误 */
			/* 用户自定义错误处理 */
		}
	}
#endif

	while (1) {	
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);
		/* 构造超时时间 */
		timeout.tv_sec = 0;
		timeout.tv_usec = 500000; /* 500ms */
		ret = select(fd + 1, &readfds, NULL, NULL, &timeout);
		switch (ret) {
			case 0: 	/* 超时 */
				/* 用户自定义超时处理 */
				break;
			case -1:	/* 错误 */
				/* 用户自定义错误处理 */
				break;
			default:  /* 可以读取数据 */
				if(FD_ISSET(fd, &readfds)) {
					ret = read(fd, &data, sizeof(data));
					if (ret < 0) {
						/* 读取错误 */
					} else {
						if (data)
							printf("key value=%d\r\n", data);
					}
				}
				break;
		}	
	}

	close(fd);
	return ret;
}
