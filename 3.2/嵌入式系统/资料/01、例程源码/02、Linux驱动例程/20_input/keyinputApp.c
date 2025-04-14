#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/ioctl.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include <poll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <linux/input.h>
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: keyinputApp.c
作者	  	: 左忠凯
版本	   	: V1.0
描述	   	: input子系统测试APP。
其他	   	: 无
使用方法	 ：./keyinputApp /dev/input/event1
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/8/26 左忠凯创建
***************************************************************/

/* 定义一个input_event变量，存放输入事件信息 */
static struct input_event inputevent;

/*
 * @description		: main主程序
 * @param - argc 	: argv数组元素个数
 * @param - argv 	: 具体参数
 * @return 			: 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
	int fd;
	int err = 0;
	char *filename;

	filename = argv[1];

	if(argc != 2) {
		printf("Error Usage!\r\n");
		return -1;
	}

	fd = open(filename, O_RDWR);
	if (fd < 0) {
		printf("Can't open file %s\r\n", filename);
		return -1;
	}

	while (1) {
		err = read(fd, &inputevent, sizeof(inputevent));
		if (err > 0) { /* 读取数据成功 */
			switch (inputevent.type) {


				case EV_KEY:
					if (inputevent.code < BTN_MISC) { /* 键盘键值 */
						printf("key %d %s\r\n", inputevent.code, inputevent.value ? "press" : "release");
					} else {
						printf("button %d %s\r\n", inputevent.code, inputevent.value ? "press" : "release");
					}
					break;

				/* 其他类型的事件，自行处理 */
				case EV_REL:
					break;
				case EV_ABS:
					break;
				case EV_MSC:
					break;
				case EV_SW:
					break;
			}
		} else {
			printf("读取数据失败\r\n");
		}
	}
	return 0;
}

