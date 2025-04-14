/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : lcd_vertical_display.c
 作者 : 邓涛
 版本 : V1.0
 描述 : FrameBuffer应用编程之横屏切换为竖屏显示
 其他 : 无
 论坛 : www.openedv.com
 日志 : 初版 V1.0 2021/6/15 邓涛创建
 ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

#define argb8888_to_rgb565(color)   ({ \
            unsigned int temp = (color); \
            ((temp & 0xF80000UL) >> 8) | \
            ((temp & 0xFC00UL) >> 5) | \
            ((temp & 0xF8UL) >> 3); \
            })

static int lcd_width;                   //LCD X分辨率
static int lcd_height;                  //LCD Y分辨率
static int lcd_max_y;                   //LCD Y坐标最大值
static int user_width;                  //竖屏模式下X分辨率
static int user_height;                 //竖屏模式下Y分辨率
static unsigned short *screen_base = NULL;      //映射后的显存基地址

/********************************************************************
 * 函数名称： lcd_draw_point
 * 功能描述： 打点
 * 输入参数： x, y, color
 * 返 回 值： 无
 ********************************************************************/
static void lcd_draw_point(unsigned int x, unsigned int y, unsigned int color)
{
    unsigned short rgb565_color = argb8888_to_rgb565(color);//得到RGB565颜色值

    /* 对传入参数的校验 */
    if (x >= user_width)
        x = user_width - 1;
    if (y >= user_height)
        y = user_height - 1;

    /* 填充颜色 */
    screen_base[(lcd_max_y-x) * lcd_width + y] = rgb565_color;
}

/********************************************************************
 * 函数名称： lcd_draw_line
 * 功能描述： 画线（水平或垂直线）
 * 输入参数： x, y, dir, length, color
 * 返 回 值： 无
 ********************************************************************/
static void lcd_draw_line(unsigned int x, unsigned int y, int dir,
            unsigned int length, unsigned int color)
{
    unsigned short rgb565_color = argb8888_to_rgb565(color);//得到RGB565颜色值
    unsigned int end;
    unsigned long temp;

    /* 对传入参数的校验 */
    if (x >= user_width)
        x = user_width - 1;
    if (y >= user_height)
        y = user_height - 1;

    /* 填充颜色 */
    temp = (lcd_max_y-x) * lcd_width + y;
    if (dir) {  //水平线
        end = x + length - 1;
        if (end >= user_width)
            end = user_width - 1;

        for ( ; x <= end; x++, temp -= lcd_width)
            screen_base[temp] = rgb565_color;
    }
    else {  //垂直线
        end = y + length - 1;
        if (end >= user_height)
            end = user_height - 1;

        for ( ; y <= end; y++, temp++)
            screen_base[temp] = rgb565_color;
    }
}

/********************************************************************
 * 函数名称： lcd_draw_rectangle
 * 功能描述： 画矩形
 * 输入参数： start_x, end_x, start_y, end_y, color
 * 返 回 值： 无
 ********************************************************************/
static void lcd_draw_rectangle(unsigned int start_x, unsigned int end_x,
            unsigned int start_y, unsigned int end_y,
            unsigned int color)
{
    int x_len = end_x - start_x + 1;
    int y_len = end_y - start_y - 1;

    lcd_draw_line(start_x, start_y, 1, x_len, color);//上边
    lcd_draw_line(start_x, end_y, 1, x_len, color); //下边
    lcd_draw_line(start_x, start_y + 1, 0, y_len, color);//左边
    lcd_draw_line(end_x, start_y + 1, 0, y_len, color);//右边
}

/********************************************************************
 * 函数名称： lcd_fill
 * 功能描述： 将一个矩形区域填充为参数color所指定的颜色
 * 输入参数： start_x, end_x, start_y, end_y, color
 * 返 回 值： 无
 ********************************************************************/
static void lcd_fill(unsigned int start_x, unsigned int end_x,
            unsigned int start_y, unsigned int end_y,
            unsigned int color)
{
    unsigned short rgb565_color = argb8888_to_rgb565(color);//得到RGB565颜色值
    unsigned long temp;
    unsigned long step_size_count;
    int x;

    /* 对传入参数的校验 */
    if (end_x >= user_width)
        end_x = user_width - 1;
    if (end_y >= user_height)
        end_y = user_height - 1;

    /* 填充颜色 */
    temp = (lcd_max_y-start_x) * lcd_width + start_y;
    for ( ; start_y <= end_y; start_y++, temp++) {

        step_size_count = 0;
        for (x = start_x; x <= end_x; x++, step_size_count += lcd_width)
            screen_base[temp - step_size_count] = rgb565_color;
    }
}

int main(int argc, char *argv[])
{
    struct fb_fix_screeninfo fb_fix;
    struct fb_var_screeninfo fb_var;
    unsigned int screen_size;
    int fd;

    /* 打开framebuffer设备 */
    if (0 > (fd = open("/dev/fb0", O_RDWR))) {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    /* 获取参数信息 */
    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);

    screen_size = fb_fix.line_length * fb_var.yres;
    lcd_width = fb_var.xres;
    lcd_height = fb_var.yres;
    lcd_max_y = lcd_height - 1;
    user_width = fb_var.yres;
    user_height = fb_var.xres;

    /* 将显示缓冲区映射到进程地址空间 */
    screen_base = mmap(NULL, screen_size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (MAP_FAILED == (void *)screen_base) {
        perror("mmap error");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* 画正方形方块 */
    int w = user_height * 0.25;//方块的宽度为1/4屏幕高度
    lcd_fill(0, user_width-1, 0, user_height-1, 0x0); //清屏（屏幕显示黑色）
    lcd_fill(0, w, 0, w, 0xFF0000); //红色方块
    lcd_fill(user_width-w, user_width-1, 0, w, 0xFF00);   //绿色方块
    lcd_fill(0, w, user_height-w, user_height-1, 0xFF);   //蓝色方块
    lcd_fill(user_width-w, user_width-1, user_height-w, user_height-1, 0xFFFF00);//黄色方块

    /* 画线: 十字交叉线 */
    lcd_draw_line(0, user_height * 0.5, 1, user_width, 0xFFFFFF);//白色水平线
    lcd_draw_line(user_width * 0.5, 0, 0, user_height, 0xFFFFFF);//白色垂直线

    /* 画矩形 */
    unsigned int s_x, s_y, e_x, e_y;
    s_x = 0.25 * user_width;
    s_y = w;
    e_x = user_width - s_x;
    e_y = user_height - s_y;

    for ( ; (s_x <= e_x) && (s_y <= e_y);
            s_x+=5, s_y+=5, e_x-=5, e_y-=5)
        lcd_draw_rectangle(s_x, e_x, s_y, e_y, 0xFFFFFF);

    /* 退出 */
    munmap(screen_base, screen_size);  //取消映射
    close(fd);  //关闭文件
    exit(EXIT_SUCCESS);    //退出进程
}
