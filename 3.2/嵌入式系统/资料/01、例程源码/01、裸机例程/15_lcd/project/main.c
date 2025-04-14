/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 mian.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : I.MX6U开发板裸机实验16 LCD液晶屏实验
其他	   : 本实验学习如何在I.MX6U上驱动RGB LCD液晶屏幕，I.MX6U有个
 		 ELCDIF接口，通过此接口可以连接一个RGB LCD液晶屏。
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
#include "stdio.h"
#include "bsp_lcd.h"
#include "bsp_lcdapi.h"


/* 背景颜色索引 */
unsigned int backcolor[10] = {
	LCD_BLUE, 		LCD_GREEN, 		LCD_RED, 	LCD_CYAN, 	LCD_YELLOW, 
	LCD_LIGHTBLUE, 	LCD_DARKBLUE, 	LCD_WHITE, 	LCD_BLACK, 	LCD_ORANGE

}; 
	

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	unsigned char index = 0;
	unsigned char state = OFF;

	int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 			*/
	delay_init();				/* 初始化延时 			*/
	clk_enable();				/* 使能所有的时钟 			*/
	led_init();					/* 初始化led 			*/
	beep_init();				/* 初始化beep	 		*/
	uart_init();				/* 初始化串口，波特率115200 */
	lcd_init();					/* 初始化LCD 			*/

	tftlcd_dev.forecolor = LCD_RED;	  
	while(1)				
	{	
		lcd_clear(backcolor[index]);
		delayms(1); 	  
		lcd_show_string(10, 40, 260, 32, 32,(char*)"ALPHA IMX6U"); 	
		lcd_show_string(10, 80, 240, 24, 24,(char*)"RGBLCD TEST");
		lcd_show_string(10, 110, 240, 16, 16,(char*)"ATOM@ALIENTEK");      					 
		lcd_show_string(10, 130, 240, 12, 12,(char*)"2019/8/14");	      					 
	    index++;
		if(index == 10)
			index = 0;      
		state = !state;
		led_switch(LED0,state);
		delayms(1000);

#if 0
		index++;
		if(index == 10)
			index = 0;
		lcd_fill(0, 300, 1023, 599, backcolor[index]);
		lcd_show_string(800,10,240,32,32,(char*)"INDEX=");  /*显示字符串				  */
		lcd_shownum(896,10, index, 2, 32); 					/* 显示数字，叠加显示	*/
		
		state = !state;
		led_switch(LED0,state);
		delayms(1000);	/* 延时一秒	*/
#endif
	}
	return 0;
}
