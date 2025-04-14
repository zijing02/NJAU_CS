/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : freetype_test.c
 作者 : 邓涛
 版本 : V1.0
 描述 : FreeType测试示例代码
 其他 : 无
 论坛 : www.openedv.com
 日志 : 初版 V1.0 2021/7/14 邓涛创建
 ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <math.h>       //数学库函数头文件

#include <wchar.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define FB_DEV      "/dev/fb0"      //LCD设备节点

#define argb8888_to_rgb565(color)   ({ \
            unsigned int temp = (color); \
            ((temp & 0xF80000UL) >> 8) | \
            ((temp & 0xFC00UL) >> 5) | \
            ((temp & 0xF8UL) >> 3); \
            })

static unsigned int width;                       //LCD宽度
static unsigned int height;                      //LCD高度
static unsigned short *screen_base = NULL;//LCD显存基地址 RGB565
static unsigned long screen_size;
static int fd = -1;

static FT_Library library;
static FT_Face face;

static int fb_dev_init(void)
{
    struct fb_var_screeninfo fb_var = {0};
    struct fb_fix_screeninfo fb_fix = {0};

    /* 打开framebuffer设备 */
    fd = open(FB_DEV, O_RDWR);
    if (0 > fd) {
        fprintf(stderr, "open error: %s: %s\n", FB_DEV, strerror(errno));
        return -1;
    }

    /* 获取framebuffer设备信息 */
    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);

    screen_size = fb_fix.line_length * fb_var.yres;
    width = fb_var.xres;
    height = fb_var.yres;

    /* 内存映射 */
    screen_base = mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (MAP_FAILED == (void *)screen_base) {
        perror("mmap error");
        close(fd);
        return -1;
    }

    /* LCD背景刷成黑色 */
    memset(screen_base, 0xFF, screen_size);
    return 0;
}

static int freetype_init(const char *font, int angle)
{
    FT_Error error;
    FT_Vector pen;
    FT_Matrix matrix;
    float rad;      //旋转角度

    /* FreeType初始化 */
    FT_Init_FreeType(&library);

    /* 加载face对象 */
    error = FT_New_Face(library, font, 0, &face);
    if (error) {
        fprintf(stderr, "FT_New_Face error: %d\n", error);
        exit(EXIT_FAILURE);
    }

    /* 原点坐标 */
    pen.x = 0 * 64;
    pen.y = 0 * 64;     //原点设置为(0, 0)

    /* 2x2矩阵初始化 */
    rad = (1.0 * angle / 180) * M_PI;   //（角度转换为弧度）M_PI是圆周率
#if 0       //非水平方向
    matrix.xx = (FT_Fixed)( cos(rad) * 0x10000L);
    matrix.xy = (FT_Fixed)(-sin(rad) * 0x10000L);
    matrix.yx = (FT_Fixed)( sin(rad) * 0x10000L);
    matrix.yy = (FT_Fixed)( cos(rad) * 0x10000L);
#endif

#if 1       //斜体  水平方向显示的
    matrix.xx = (FT_Fixed)( cos(rad) * 0x10000L);
    matrix.xy = (FT_Fixed)( sin(rad) * 0x10000L);
    matrix.yx = (FT_Fixed)( 0 * 0x10000L);
    matrix.yy = (FT_Fixed)( 1 * 0x10000L);
#endif

    /* 设置 */
    FT_Set_Transform(face, &matrix, &pen);
    FT_Set_Pixel_Sizes(face, 50, 0);    //设置字体大小

    return 0;
}

static void lcd_draw_character(int x, int y,
            const wchar_t *str, unsigned int color)
{
    unsigned short rgb565_color = argb8888_to_rgb565(color);//得到RGB565颜色值
    FT_GlyphSlot slot = face->glyph;
    size_t len = wcslen(str);   //计算字符的个数
    long int temp;
    int n;
    int i, j, p, q;
    int max_x, max_y, start_y, start_x;

    // 循环加载各个字符
    for (n = 0; n < len; n++) {

        // 加载字形、转换得到位图数据
        if (FT_Load_Char(face, str[n], FT_LOAD_RENDER))
            continue;

        start_y = y - slot->bitmap_top; //计算字形轮廓上边y坐标起点位置 注意是减去bitmap_top
        if (0 > start_y) {//如果为负数 如何处理？？
            q = -start_y;
            temp = 0;
            j = 0;
        }
        else {      // 正数又该如何处理??
            q = 0;
            temp = width * start_y;
            j = start_y;
        }

        max_y = start_y + slot->bitmap.rows;//计算字形轮廓下边y坐标结束位置
        if (max_y > (int)height)
            max_y = height;

        for (; j < max_y; j++, q++, temp += width) {

            start_x = x + slot->bitmap_left;  //起点位置要加上左边空余部分长度
            if (0 > start_x) {
                p = -start_x;
                i = 0;
            }
            else {
                p = 0;
                i = start_x;
            }

            max_x = start_x + slot->bitmap.width;
            if (max_x > (int)width)
                max_x = width;

            for (; i < max_x; i++, p++) {

                // 如果数据不为0，则表示需要填充颜色
                if (slot->bitmap.buffer[q * slot->bitmap.width + p])
                    screen_base[temp + i] = rgb565_color;
            }
        }

        //调整到下一个字形的原点
        x += slot->advance.x / 64;  //26.6固定浮点格式
        y -= slot->advance.y / 64;
    }
}

int main(int argc, char *argv[])
{
    

    /* LCD初始化 */
    if (fb_dev_init())
        exit(EXIT_FAILURE);

    /* freetype初始化 */
    if (freetype_init(argv[1], atoi(argv[2])))
        exit(EXIT_FAILURE);

    /* 在LCD上显示中文 */
    int y = height * 0.25;
    lcd_draw_character(50, 100, L"路漫漫其修远兮，吾将上下而求索", 0x000000);
    lcd_draw_character(50, y+100, L"莫愁前路无知己，天下谁人不识君", 0x9900FF);
    lcd_draw_character(50, 2*y+100, L"君不见黄河之水天上来，奔流到海不复回", 0xFF0099);
    lcd_draw_character(50, 3*y+100, L"君不见高堂明镜悲白发，朝如青丝暮成雪", 0x9932CC);

    /* 退出程序 */
    FT_Done_Face(face);
    FT_Done_FreeType(library);
    munmap(screen_base, screen_size);
    close(fd);
    exit(EXIT_SUCCESS);
}
