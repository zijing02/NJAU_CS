/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 mian.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : I.MX6U开发板裸机实验4 使用NXP提供的I.MX6ULL官方IAR SDK包开发
其他	   : 前面其他所有实验中，寄存器定义都是我们自己手写的，但是I.MX6U
		 的寄存器有很多，全部自己写太费时间，而且没意义。NXP官方提供了
		 针对I.MX6ULL的SDK开发包，是基于IAR环境的，这个SDK包里面已经提
		 供了I.MX6ULL所有相关寄存器定义，虽然是针对I.MX6ULL编写的，但是同样
		 适用于I.MX6UL。本节我们就将相关的寄存器定义文件移植到Linux环境下，
		 要移植的文件有：
		 fsl_common.h
		 fsl_iomuxc.h
		 MCIMX6Y2.h
		 自定义文件 cc.h
日志	   : 初版V1.0 2019/1/3 左忠凯创建
**************************************************************/
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"

/*
 * @description	: 使能I.MX6U所有外设时钟
 * @param 		: 无
 * @return 		: 无
 */
void clk_enable(void)
{
	CCM->CCGR0 = 0XFFFFFFFF;
	CCM->CCGR1 = 0XFFFFFFFF;

	CCM->CCGR2 = 0XFFFFFFFF;
	CCM->CCGR3 = 0XFFFFFFFF;
	CCM->CCGR4 = 0XFFFFFFFF;
	CCM->CCGR5 = 0XFFFFFFFF;
	CCM->CCGR6 = 0XFFFFFFFF;

}

/*
 * @description	: 初始化LED对应的GPIO
 * @param 		: 无
 * @return 		: 无
 */
void led_init(void)
{
	/* 1、初始化IO复用 */
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03,0);		/* 复用为GPIO1_IO0 */

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
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03,0X10B0);

	/* 3、初始化GPIO,设置GPIO1_IO03设置为输出  */
	GPIO1->GDIR |= (1 << 3);	
	
	/* 4、设置GPIO1_IO03输出低电平，打开LED0 */
	GPIO1->DR &= ~(1 << 3);			
}

/*
 * @description	: 打开LED灯
 * @param 		: 无
 * @return 		: 无
 */
void led_on(void)
{
	/* 将GPIO1_DR的bit3清零 	*/
	GPIO1->DR &= ~(1<<3); 
}

/*
 * @description	: 关闭LED灯
 * @param 		: 无
 * @return 		: 无
 */
void led_off(void)
{
	/* 将GPIO1_DR的bit3置1 */
	GPIO1->DR |= (1<<3); 
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
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	clk_enable();		/* 使能所有的时钟 			*/
	led_init();			/* 初始化led 			*/

	while(1)			/* 死循环 				*/
	{	
		led_off();		/* 关闭LED 			*/
		delay(500);		/* 延时500ms 			*/

		led_on();		/* 打开LED 			*/
		delay(500);		/* 延时500ms 			*/
	}

	return 0;
}
