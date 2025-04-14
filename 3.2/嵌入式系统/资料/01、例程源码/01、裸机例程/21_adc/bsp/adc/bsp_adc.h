#ifndef __ADC_H
#define __ADC_H
/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_adc.h
作者	   : 左忠凯
版本	   : V1.0
描述	   : ADC驱动头文件。
其他	   : 无
论坛 	   : www.openedv.com
日志	   : 初版V1.0 2019/1/22 左忠凯创建
***************************************************************/
#include "imx6ul.h"

int adc1ch1_init(void);
status_t adc1_autocalibration(void);
uint32_t getadc_value(void);
unsigned short getadc_average(unsigned char times);
unsigned short getadc_volt(void);

#endif

