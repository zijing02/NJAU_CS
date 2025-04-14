/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: ap3216cApp.c
作者	  	: 正点原子Linux团队
版本	   	: V1.0
描述	   	: ap3216c设备iio框架测试程序。
其他	   	: 无
使用方法	 ：./ap3216cApp
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2021/03/19 正点原子Linux团队创建
***************************************************************/
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
#include <errno.h>

/* 字符串转数字，将浮点小数字符串转换为浮点数数值 */
#define SENSOR_FLOAT_DATA_GET(ret, index, str, member)\
	ret = file_data_read(file_path[index], str);\
	dev->member = atof(str);\
	
/* 字符串转数字，将整数字符串转换为整数数值 */
#define SENSOR_INT_DATA_GET(ret, index, str, member)\
	ret = file_data_read(file_path[index], str);\
	dev->member = atoi(str);\


/* ap3216c iio框架对应的文件路径 */
static char *file_path[] = {
	"/sys/bus/iio/devices/iio:device1/in_intensity_both_scale",
	"/sys/bus/iio/devices/iio:device1/in_intensity_both_raw",
	"/sys/bus/iio/devices/iio:device1/in_intensity_ir_raw",
	"/sys/bus/iio/devices/iio:device1/in_proximity_raw",
};

/* 文件路径索引，要和file_path里面的文件顺序对应 */
enum path_index {
	IN_INTENSITY_BOTH_SCALE = 0,
	IN_INTENSITY_BOTH_RAW,
	IN_INTENSITY_IR_RAW,
	IN_PROXIMITY_RAW,
};

/*
 * ap3216c数据设备结构体
 */
struct ap3216c_dev{
	int als_raw, ir_raw, ps_raw;
	float als_scale;
	float als_act;
};

struct ap3216c_dev ap3216c;

 /*
 * @description			: 读取指定文件内容
 * @param - filename 	: 要读取的文件路径
 * @param - str 		: 读取到的文件字符串
 * @return 				: 0 成功;其他 失败
 */
static int file_data_read(char *filename, char *str)
{
	int ret = 0;
	FILE *data_stream;

    data_stream = fopen(filename, "r"); /* 只读打开 */
    if(data_stream == NULL) {
		printf("can't open file %s\r\n", filename);
		return -1;
	}

	ret = fscanf(data_stream, "%s", str);
    if(!ret) {
        printf("file read error!\r\n");
    } else if(ret == EOF) {
        /* 读到文件末尾的话将文件指针重新调整到文件头 */
        fseek(data_stream, 0, SEEK_SET);  
    }
	fclose(data_stream);	/* 关闭文件 */	
	return 0;
}

 /*
 * @description	: 获取AP3216C数据
 * @param - dev : 设备结构体
 * @return 		: 0 成功;其他 失败
 */
static int sensor_read(struct ap3216c_dev *dev)
{
	int ret = 0;
	char str[50];

	SENSOR_FLOAT_DATA_GET(ret, IN_INTENSITY_BOTH_SCALE, str, als_scale);
	SENSOR_INT_DATA_GET(ret, IN_INTENSITY_BOTH_RAW, str, als_raw);
	SENSOR_INT_DATA_GET(ret, IN_INTENSITY_IR_RAW, str, ir_raw);
	SENSOR_INT_DATA_GET(ret, IN_PROXIMITY_RAW, str, ps_raw);

	/* 将ALS转换为实际lux */
	dev->als_act = dev->als_scale * dev->als_raw;
	return ret;
}

/*
 * @description		: main主程序
 * @param - argc 	: argv数组元素个数
 * @param - argv 	: 具体参数
 * @return 			: 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
	int ret = 0;

	if (argc != 1) {
		printf("Error Usage!\r\n");
		return -1;
	}

	while (1) {
		ret = sensor_read(&ap3216c);
		if(ret == 0) { 			/* 数据读取成功 */
			printf("\r\n原始值:\r\n");
			printf("als = %d, ps = %d, ir = %d\r\n", ap3216c.als_raw, ap3216c.ps_raw, ap3216c.ir_raw);
			printf("实际值:");
			printf("act als = %.2f lx\r\n", ap3216c.als_act);
		}
		usleep(100000); /*100ms */
	}
	return 0;
}

