/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_adc.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : ADC驱动文件。
其他	   : 无
论坛 	   : www.openedv.com
日志	   : 初版V1.0 2019/1/22 左忠凯创建
***************************************************************/
#include "bsp_adc.h"
#include "bsp_delay.h"
#include "stdio.h"

/*
 * @description	: 初始化ADC1_CH1，使用GPIO1_IO01这个引脚。
 * @param		: 无
 * @return 		: 0 成功，其他值 错误代码
 */
int adc1ch1_init(void)
{
    int ret = 0;

    /* 1、初始化ADC1 CH1 */
    /* CFG寄存器
     * bit16        0       关闭复写功能
     * bit15:14     00      硬件平均设置为默认值，00的时候4次平均，
     *                      但是得ADC_GC寄存器的AVGE位置1来使能硬件平均
     * bit13        0       软件触发
     * bit12:1      00      参考电压为VREFH/VREFL，也就是3.3V/0V
     * bit10        0       正常转换速度
     * bit9:8       00      采样时间2/12，ADLSMP=0(短采样)的时候为2个周期
     *                      ADLSMP=1(长采样)的时候为12个周期
     * bit7         0       非低功耗模式
     * bit6:5       00      ADC时钟源1分频 
     * bit4         0       短采样
     * bit3:2       10      12位ADC
     * bit1:0       11      ADC时钟源选择ADACK   
     */
    ADC1->CFG = 0;
    ADC1->CFG |= (2 << 2) | (3 << 0);

    /* GC寄存器
     * bit7     0       先关闭校准功能，后面会校准
     * bit6     0       关闭持续转换
     * bit5     0       关闭硬件平均功能
     * bit4     0       关闭比较功能
     * bit3     0       关闭比较的Greater Than功能
     * bit2     0       关闭比较的Range功能
     * bit1     0       关闭DMA
     * bit0     1       使能ADACK
     */
    ADC1->GC = 0;
    ADC1->GC |= 1 << 0;

    /* 2、校准ADC */
    if(adc1_autocalibration() != kStatus_Success)
        ret = -1;

    return ret;
}

/*
 * @description	: 初始化ADC1校准
 * @param		: 无
 * @return 		: kStatus_Success 成功，kStatus_Fail 失败
 */
status_t adc1_autocalibration(void)
{
    status_t ret  = kStatus_Success;

    ADC1->GS |= (1 << 2);   /* 清除CALF位，写1清零 */
    ADC1->GC |= (1 << 7);   /* 使能校准功能 */

    /* 校准完成之前GC寄存器的CAL位会一直为1，直到校准完成此位自动清零 */
    while((ADC1->GC & (1 << 7)) != 0) { 
        /* 如果GS寄存器的CALF位为1的话表示校准失败 */
        if((ADC1->GS & (1 << 2)) != 0) {
            ret = kStatus_Fail;
            break;
        }
    }

    /* 校准成功以后HS寄存器的COCO0位会置1 */
    if((ADC1->HS  & (1 << 0)) == 0) 
        ret = kStatus_Fail;

    /* 如果GS寄存器的CALF位为1的话表示校准失败 */
        if((ADC1->GS & (1 << 2)) != 0) 
        ret = kStatus_Fail;

    return ret;
}

/*
 * @description	: 获取ADC原始值
 * @param		: 无
 * @return 		: 获取到的ADC原始值
 */
unsigned int getadc_value(void)
{

    /* 配置ADC通道1 */
    ADC1->HC[0] = 0;            /* 关闭转换结束中断    */
    ADC1->HC[0] |= (1 << 0);     /* 通道1            */

    while((ADC1->HS & (1 << 0)) == 0);  /* 等待转换完成 */

    return ADC1->R[0];    /* 返回ADC值 */
}

/*
 * @description	        : 获取ADC平均值
 * @param		times   : 获取次数
 * @return 		        : times次转换结果平均值
 */
unsigned short getadc_average(unsigned char times)
{
	unsigned int temp_val = 0;
	unsigned char t;
	for(t = 0; t < times; t++){
		temp_val += getadc_value();
		delayms(5);
	}
	return temp_val / times;
} 

/*
 * @description : 获取ADC对应的电压值
 * @param	    : 无
 * @return 	    : 获取到的电压值，单位为mV
 */
unsigned short getadc_volt(void)
{
    unsigned int adcvalue=0;
    unsigned int ret = 0;
    adcvalue = getadc_average(5);
    ret = (float)adcvalue * (3300.0f / 4096.0f);    	/* 获取计算后的带小数的实际电压值 */
    return  ret;
}

