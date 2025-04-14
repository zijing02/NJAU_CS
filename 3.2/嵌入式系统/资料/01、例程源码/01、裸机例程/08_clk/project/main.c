/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 mian.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : I.MX6U开发板裸机实验8 系统时钟配置实验
其他	   : 本实验主要学习如何配置I.MX6U的系统时钟，I.MX6UL内部boot
		 代码默认将主频设置为396M，但是I.MX6U标准主频为528MHz(最高
		 可到696MHz)，本实验就讲解如何将I.MX6U的主频设置到528MHz
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/4 左忠凯创建
**************************************************************/
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	int i = 0;
	int keyvalue = 0;
	unsigned char led_state = OFF;
	unsigned char beep_state = OFF;

	imx6u_clkinit();	/* 初始化系统时钟 			*/
	clk_enable();		/* 使能所有的时钟 			*/
	led_init();			/* 初始化led 			*/
	beep_init();		/* 初始化beep	 		*/
	key_init();			/* 初始化key 			*/

	while(1)			
	{	
		keyvalue = key_getvalue();
		if(keyvalue)
		{
			switch ((keyvalue))
			{
				case KEY0_VALUE:
					beep_state = !beep_state;
					beep_switch(beep_state);
					break;
			}
		}
		i++;
		if(i==50)
		{
			i = 0;
			led_state = !led_state;
			led_switch(LED0, led_state);
		}

		delay(10);
	}

	return 0;
}
