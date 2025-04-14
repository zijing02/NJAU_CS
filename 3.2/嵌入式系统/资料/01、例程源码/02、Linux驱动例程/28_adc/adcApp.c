/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: adcApp.c
作者	  	: 正点原子Linux团队
版本	   	: V1.0
描述	   	: adc测试应用文件。
其他	   	: 无
使用方法	 ：./adcApp
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2021/09/24 正点原子Linux团队创建
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


/* adc iio框架对应的文件路径 */
static char *file_path[] = {
	"/sys/bus/iio/devices/iio:device0/in_voltage_scale",
	"/sys/bus/iio/devices/iio:device0/in_voltage1_raw",
};

/* 文件路径索引，要和file_path里面的文件顺序对应 */
enum path_index {
	IN_VOLTAGE_SCALE = 0,
	IN_VOLTAGE_RAW,
};

/*
 * ADC数据设备结构体
 */
struct adc_dev{
	int raw;
	float scale;
	float act;
};

struct adc_dev imx6ulladc;

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
 * @description	: 获取ADC数据
 * @param - dev : 设备结构体
 * @return 		: 0 成功;其他 失败
 */
static int adc_read(struct adc_dev *dev)
{
	int ret = 0;
	char str[50];

	SENSOR_FLOAT_DATA_GET(ret, IN_VOLTAGE_SCALE, str, scale);
	SENSOR_INT_DATA_GET(ret, IN_VOLTAGE_RAW, str, raw);

	/* 转换得到实际电压值mV */
	dev->act = (dev->scale * dev->raw)/1000.f;
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
		ret = adc_read(&imx6ulladc);
		if(ret == 0) { 			/* 数据读取成功 */
			printf("ADC原始值：%d，电压值：%.3fV\r\n", imx6ulladc.raw, imx6ulladc.act);
		}
		usleep(100000); /*100ms */
	}
	return 0;
}
 