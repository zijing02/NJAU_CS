/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : show_png_image.c
 作者 : 邓涛
 版本 : V1.0
 描述 : libpng使用实战
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
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <png.h>

static int width;                       //LCD X分辨率
static int height;                      //LCD Y分辨率
static unsigned short *screen_base = NULL;        //映射后的显存基地址
static unsigned long line_length;       //LCD一行的长度（字节为单位）
static unsigned int bpp;    //像素深度bpp

static int show_png_image(const char *path)
{
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    FILE *png_file = NULL;
    unsigned short *fb_line_buf = NULL; //行缓冲区:用于存储写入到LCD显存的一行数据
    unsigned int min_h, min_w;
    unsigned int valid_bytes;
    unsigned int image_h, image_w;
    png_bytepp row_pointers = NULL;
    int i, j, k;

    /* 打开png文件 */
    png_file = fopen(path, "r");    //以只读方式打开
    if (NULL == png_file) {
        perror("fopen error");
        return -1;
    }

    /* 分配和初始化png_ptr、info_ptr */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(png_file);
        return -1;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(png_file);
        return -1;
    }

    /* 设置错误返回点 */
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);
        return -1;
    }

    /* 指定数据源 */
    png_init_io(png_ptr, png_file);

    /* 读取png文件 */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_ALPHA, NULL);
    image_h = png_get_image_height(png_ptr, info_ptr);
    image_w = png_get_image_width(png_ptr, info_ptr);
    printf("分辨率: %d*%d\n", image_w, image_h);

    /* 判断是不是RGB888 */
    if ((8 != png_get_bit_depth(png_ptr, info_ptr)) &&
        (PNG_COLOR_TYPE_RGB != png_get_color_type(png_ptr, info_ptr))) {
        printf("Error: Not 8bit depth or not RGB color");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);
        return -1;
    }

    /* 判断图像和LCD屏那个的分辨率更低 */
    if (image_w > width)
        min_w = width;
    else
        min_w = image_w;

    if (image_h > height)
        min_h = height;
    else
        min_h = image_h;

    valid_bytes = min_w * bpp / 8;

    /* 读取解码后的数据 */
    fb_line_buf = malloc(valid_bytes);
    row_pointers = png_get_rows(png_ptr, info_ptr);//获取数据

    unsigned int temp = min_w * 3;  //RGB888 一个像素3个bit位
    for(i = 0; i < min_h; i++) {

        // RGB888转为RGB565
        for(j = k = 0; j < temp; j += 3, k++)
            fb_line_buf[k] = ((row_pointers[i][j] & 0xF8) << 8) |
                ((row_pointers[i][j+1] & 0xFC) << 3) |
                ((row_pointers[i][j+2] & 0xF8) >> 3);

        memcpy(screen_base, fb_line_buf, valid_bytes);//将一行数据刷入显存
        screen_base += width;   //定位到显存下一行
    }

    /* 结束、销毁/释放内存 */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    free(fb_line_buf);
    fclose(png_file);
    return 0;
}

int main(int argc, char *argv[])
{
    struct fb_fix_screeninfo fb_fix;
    struct fb_var_screeninfo fb_var;
    unsigned int screen_size;
    int fd;

    /* 传参校验 */
    if (2 != argc) {
        fprintf(stderr, "usage: %s <png_file>\n", argv[0]);
        exit(-1);
    }

    /* 打开framebuffer设备 */
    if (0 > (fd = open("/dev/fb0", O_RDWR))) {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    /* 获取参数信息 */
    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);

    line_length = fb_fix.line_length;
    bpp = fb_var.bits_per_pixel;
    screen_size = line_length * fb_var.yres;
    width = fb_var.xres;
    height = fb_var.yres;

    /* 将显示缓冲区映射到进程地址空间 */
    screen_base = mmap(NULL, screen_size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (MAP_FAILED == (void *)screen_base) {
        perror("mmap error");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* 显示BMP图片 */
    memset(screen_base, 0xFF, screen_size);//屏幕刷白
    show_png_image(argv[1]);

    /* 退出 */
    munmap(screen_base, screen_size);  //取消映射
    close(fd);  //关闭文件
    exit(EXIT_SUCCESS);    //退出进程
}
