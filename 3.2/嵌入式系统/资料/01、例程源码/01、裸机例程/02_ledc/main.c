/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 mian.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : I.MX6U开发板裸机实验2 C语言点灯
		 使用C语言来点亮开发板上的LED灯，学习和掌握如何用C语言来
		 完成对I.MX6U处理器的GPIO初始化和控制。
其他	   : 无
日志	   : 初版V1.0 2019/1/3 左忠凯创建
**************************************************************/
#include "main.h"

/*
 * @description	: 使能I.MX6U所有外设时钟
 * @param 		: 无
 * @return 		: 无
 */
void clk_enable(void)
{
	CCM_CCGR0 = 0xffffffff;
	CCM_CCGR1 = 0xffffffff;
	CCM_CCGR2 = 0xffffffff;
	CCM_CCGR3 = 0xffffffff;
	CCM_CCGR4 = 0xffffffff;
	CCM_CCGR5 = 0xffffffff;
	CCM_CCGR6 = 0xffffffff;
}

/*
 * @description	: 初始化LED对应的GPIO
 * @param 		: 无
 * @return 		: 无
 */
void led_init(void)
{
	/* 1、初始化IO复用 */
	SW_MUX_GPIO1_IO03 = 0x5;	/* 复用为GPIO1_IO03 */

	/* 2、、配置GPIO1_IO03的IO属性	
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 00 默认下拉
     *bit [13]: 0 kepper功能
     *bit [12]: 1 pull/keeper使能
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度100Mhz
     *bit [5:3]: 110 R0/6驱动能力
     *bit [0]: 0 低转换率
     */
	SW_PAD_GPIO1_IO03 = 0X10B0;		

	/* 3、初始化GPIO */
	GPIO1_GDIR = 0X0000008;	/* GPIO1_IO03设置为输出 */

	/* 4、设置GPIO1_IO03输出低电平，打开LED0 */
	GPIO1_DR = 0X0;
}

/*
 * @description	: 打开LED灯
 * @param 		: 无
 * @return 		: 无
 */
void led_on(void)
{
	/* 
	 * 将GPIO1_DR的bit3清零	 
	 */
	GPIO1_DR &= ~(1<<3); 
}

/*
 * @description	: 关闭LED灯
 * @param 		: 无
 * @return 		: 无
 */
void led_off(void)
{
	/*    
	 * 将GPIO1_DR的bit3置1
	 */
	GPIO1_DR |= (1<<3);
}

/*
 * @description	: 短时间延时函数
 * @param - n	: 要延时循环次数(空操作循环次数，模式延时)
 * @return 		: 无
 */
void delay_short(volatile unsigned int n)
{
	while(n--){}
}

/*
 * @description	: 延时函数,在396Mhz的主频下
 * 			  	  延时时间大约为1ms
 * @param - n	: 要延时的ms数
 * @return 		: 无
 */
void delay(volatile unsigned int n)
{
	while(n--)
	{
		delay_short(0x7ff);
	}
}

/*
 * @description	: mian函数
 * @param 	    : 无
 * @return 		: 无
 */
int main(void)
{
	clk_enable();		/* 使能所有的时钟		 	*/
	led_init();			/* 初始化led 			*/

	while(1)			/* 死循环 				*/
	{	
		led_off();		/* 关闭LED   			*/
		delay(500);		/* 延时大约500ms 		*/

		led_on();		/* 打开LED		 	*/
		delay(500);		/* 延时大约500ms 		*/
	}

	return 0;
}
