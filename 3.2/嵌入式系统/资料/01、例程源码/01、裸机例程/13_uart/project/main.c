/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 mian.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : I.MX6U开发板裸机实验13 串口实验
其他	   : 本实验我们学习如何使用I.MX6的串口，实现串口收发数据，了解
		 I.MX6的串口工作原理。
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/15 左忠凯创建
**************************************************************/
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_uart.h"

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	unsigned char a=0;
	unsigned char state = OFF;

	int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 			*/
	delay_init();				/* 初始化延时 			*/
	clk_enable();				/* 使能所有的时钟 			*/
	led_init();					/* 初始化led 			*/
	beep_init();				/* 初始化beep	 		*/
	uart_init();				/* 初始化串口，波特率115200 */

	while(1)				
	{	
		puts("请输入1个字符:");
		a=getc();
		putc(a);	//回显功能
		puts("\r\n");

		//显示输入的字符
		puts("您输入的字符为:");
		putc(a);
		puts("\r\n\r\n");
		
		state = !state;
		led_switch(LED0,state);
	}
	return 0;
}
