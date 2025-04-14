/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 mian.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : I.MX6U开发板裸机实验10 EPIT定时器实验
其他	   : 本实验主要学习使用I.MX6UL自带的EPIT定时器，学习如何使用
		 EPIT定时器来实现定时功能，巩固Cortex-A的中断知识。
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/4 左忠凯创建
**************************************************************/
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_epittimer.h"

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 			*/
	clk_enable();				/* 使能所有的时钟 			*/
	led_init();					/* 初始化led 			*/
	beep_init();				/* 初始化beep	 		*/
	key_init();					/* 初始化key 			*/
	epit1_init(0, 66000000/2);	/* 初始化EPIT1定时器，1分频
								 * 计数值为:66000000/2，也就是
								 * 定时周期为500ms。
								 */
	while(1)			
	{	
		delay(500);
	}

	return 0;
}
