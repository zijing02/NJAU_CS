/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 mian.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : I.MX6U开发板裸机实验22 ADC实验
其他	   : I.MX6ULL有ADC外设，本讲实验学习如何使用I.MX6ULL的ADC。
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/21 左忠凯创建
**************************************************************/
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_uart.h"
#include "bsp_lcd.h"
#include "bsp_lcdapi.h"
#include "bsp_rtc.h"
#include "bsp_backlight.h"
#include "bsp_adc.h"
#include "stdio.h"

/*
 * @description	: 使能I.MX6U的硬件NEON和FPU
 * @param 		: 无
 * @return 		: 无
 */
 void imx6ul_hardfpu_enable(void)
{
	uint32_t cpacr;
	uint32_t fpexc;

	/* 使能NEON和FPU */
	cpacr = __get_CPACR();
	cpacr = (cpacr & ~(CPACR_ASEDIS_Msk | CPACR_D32DIS_Msk))
		   |  (3UL << CPACR_cp10_Pos) | (3UL << CPACR_cp11_Pos);
	__set_CPACR(cpacr);
	fpexc = __get_FPEXC();
	fpexc |= 0x40000000UL;	
	__set_FPEXC(fpexc);
}

volatile 


/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	unsigned char i = 0;
	unsigned int adcvalue;
	unsigned char state = OFF;
	signed int integ; 	/* 整数部分 */
	signed int fract;	/* 小数部分 */
	
	imx6ul_hardfpu_enable();	/* 使能I.MX6U的硬件浮点 			*/
	int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 					*/
	delay_init();				/* 初始化延时 					*/
	clk_enable();				/* 使能所有的时钟 					*/
	led_init();					/* 初始化led 					*/
	beep_init();				/* 初始化beep	 				*/
	uart_init();				/* 初始化串口，波特率115200 */
	lcd_init();					/* 初始化LCD 					*/
	adc1ch1_init();				/* ADC1_CH1			 		*/ 

	tftlcd_dev.forecolor = LCD_RED;
	lcd_show_string(50, 10, 400, 24, 24, (char*)"ALPHA-IMX6U ADC TEST");  
	lcd_show_string(50, 40, 200, 16, 16, (char*)"ATOM@ALIENTEK");  
	lcd_show_string(50, 60, 200, 16, 16, (char*)"2019/12/16");   
	lcd_show_string(50, 90, 400, 16, 16, (char*)"ADC Ori Value:0000");  
	lcd_show_string(50, 110, 400, 16, 16,(char*)"ADC Val Value:0.00 V");  
	tftlcd_dev.forecolor = LCD_BLUE;
	
	while(1)					
	{
		adcvalue = getadc_average(5);
		lcd_showxnum(162, 90, adcvalue, 4, 16, 0);	/* ADC原始数据值 */
		printf("ADC orig value = %d\r\n", adcvalue);

		adcvalue = getadc_volt();
		integ = adcvalue / 1000;
		fract = adcvalue % 1000;
		lcd_showxnum(162, 110, integ, 1, 16, 0);	/* 显示电压值的整数部分，3.1111的话，这里就是显示3 */
		lcd_showxnum(178, 110, fract, 3, 16, 0X80);	/* 显示电压值小数部分（前面转换为了整形显示），这里显示的就是111. */
		printf("ADC vola = %d.%dV\r\n", integ, fract);

		delayms(50);
		i++;
		if(i == 10)
		{	
			i = 0;
			state = !state;
			led_switch(LED0,state);	
		}
	}
	return 0;
}
