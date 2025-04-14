#include "bsp_gpio.h"
/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_gpio.h
作者	   : 左忠凯
版本	   : V1.0
描述	   : GPIO操作文件。
其他	   : 无
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/4 左忠凯创建
***************************************************************/

/*
 * @description		: GPIO初始化。
 * @param - base	: 要初始化的GPIO组。
 * @param - pin		: 要初始化GPIO在组内的编号。
 * @param - config	: GPIO配置结构体。
 * @return 			: 无
 */
void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config)
{
	if(config->direction == kGPIO_DigitalInput) /* 输入 */
	{
		base->GDIR &= ~( 1 << pin);
	}
	else										/* 输出 */
	{
		base->GDIR |= 1 << pin;
		gpio_pinwrite(base,pin, config->outputLogic);/* 设置默认输出电平 */
	}
}

 /*
  * @description	 : 读取指定GPIO的电平值 。
  * @param - base	 : 要读取的GPIO组。
  * @param - pin	 : 要读取的GPIO脚号。
  * @return 		 : 无
  */
 int gpio_pinread(GPIO_Type *base, int pin)
 {
	 return (((base->DR) >> pin) & 0x1);
 }

 /*
  * @description	 : 指定GPIO输出高或者低电平 。
  * @param - base	 : 要输出的的GPIO组。
  * @param - pin	 : 要输出的GPIO脚号。
  * @param - value	 : 要输出的电平，1 输出高电平， 0 输出低低电平
  * @return 		 : 无
  */
void gpio_pinwrite(GPIO_Type *base, int pin, int value)
{
	 if (value == 0U)
	 {
		 base->DR &= ~(1U << pin); /* 输出低电平 */
	 }
	 else
	 {
		 base->DR |= (1U << pin); /* 输出高电平 */
	 }
}



