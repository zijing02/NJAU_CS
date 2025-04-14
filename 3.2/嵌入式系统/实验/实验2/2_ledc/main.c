#include "main.h"

/* 使能外设时钟 */
void clk_enable(void)
{
    CCM_CCGR1 = 0xFFFFFFFF;
    CCM_CCGR2 = 0xFFFFFFFF;
    CCM_CCGR3 = 0xFFFFFFFF;
    CCM_CCGR4 = 0xFFFFFFFF;
    CCM_CCGR5 = 0xFFFFFFFF;
    CCM_CCGR6 = 0xFFFFFFFF;
}

/* 初始化LED */
void led_init(void)
{
    SW_MUX_GPIO1_IO03 = 0x5;    /* 复用为GPIO1——IO03*/
    SW_PAD_GPIO1_IO03 = 0X10B0; /*设置GPIO1_IO03电气属性 */

    /* GPIO初始化 */
    GPIO1_GDIR = 0x8;           /* 设置为输出 */
    GPIO1_DR = 0X0;             /* 打开LED灯 */
}

/* 短延时 */
void delay_short(volatile unsigned int n)
{
    while(n--){}
}

/* 延时，一次循环大概是1ms，在主频396MHz
 * n ：延时ms数
  */
void delay(volatile unsigned int n)
{
    while(n--) {
        delay_short(0x7ff);
    }
}

/* 打开LED灯 */
void led_on(void)
{
    GPIO1_DR &= ~(1<<3); /* bit3清零 */
}

/* 关闭LED灯 */
void led_off(void)
{
    GPIO1_DR |= (1<<3); /* bit3置1 */
}

int main(void)
{
    clk_enable();   /* 使能外设时钟 */
    led_init();     /* 初始化LED */
    /* 初始化LED */
    
    /* 设置LED闪烁 */
    while(1) {
        led_on();
        delay(500);

        led_off();
        delay(500);
    }
    return 0;
}