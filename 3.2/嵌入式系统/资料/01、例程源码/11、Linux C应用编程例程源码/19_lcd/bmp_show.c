/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : show_bmp.c
 作者 : 邓涛
 版本 : V1.0
 描述 : FrameBuffer应用程序示例代码之显示BMP图像
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

/**** BMP文件头数据结构 ****/
typedef struct {
    unsigned char type[2];      //文件类型
    unsigned int size;          //文件大小
    unsigned short reserved1;   //保留字段1
    unsigned short reserved2;       //保留字段2
    unsigned int offset;        //到位图数据的偏移量
} __attribute__ ((packed)) bmp_file_header;

/**** 位图信息头数据结构 ****/
typedef struct {
    unsigned int size;          //位图信息头大小
    int width;                  //图像宽度
    int height;                 //图像高度
    unsigned short planes;          //位面数
    unsigned short bpp;         //像素深度 
    unsigned int compression;   //压缩方式
    unsigned int image_size;    //图像大小
    int x_pels_per_meter;       //像素/米
    int y_pels_per_meter;       //像素/米 
    unsigned int clr_used;
    unsigned int clr_omportant;
} __attribute__ ((packed)) bmp_info_header;

/**** 静态全局变量 ****/
static int width;                       //LCD X分辨率
static int height;                      //LCD Y分辨率
static unsigned short *screen_base = NULL;        //映射后的显存基地址
static unsigned long line_length;       //LCD一行的长度（字节为单位）

/********************************************************************
 * 函数名称： show_bmp_image
 * 功能描述： 在LCD上显示指定的BMP图片
 * 输入参数： 文件路径
 * 返 回 值： 成功返回0, 失败返回-1
 ********************************************************************/
static int show_bmp_image(const char *path)
{
    bmp_file_header file_h;
    bmp_info_header info_h;
    unsigned short *line_buf = NULL;    //行缓冲区
    unsigned long line_bytes;   //BMP图像一行的字节的大小
    unsigned int min_h, min_bytes;
    int fd = -1;
    int j;

    /* 打开文件 */
    if (0 > (fd = open(path, O_RDONLY))) {
        perror("open error");
        return -1;
    }

    /* 读取BMP文件头 */
    if (sizeof(bmp_file_header) !=
        read(fd, &file_h, sizeof(bmp_file_header))) {
        perror("read error");
        close(fd);
        return -1;
    }

    if (0 != memcmp(file_h.type, "BM", 2)) {
        fprintf(stderr, "it's not a BMP file\n");
        close(fd);
        return -1;
    }

    /* 读取位图信息头 */
    if (sizeof(bmp_info_header) !=
        read(fd, &info_h, sizeof(bmp_info_header))) {
        perror("read error");
        close(fd);
        return -1;
    }

    /* 打印信息 */
    printf("文件大小: %d\n"
         "位图数据的偏移量: %d\n"
         "位图信息头大小: %d\n"
         "图像分辨率: %d*%d\n"
         "像素深度: %d\n", file_h.size, file_h.offset,
         info_h.size, info_h.width, info_h.height,
         info_h.bpp);

    /* 将文件读写位置移动到图像数据开始处 */
    if (-1 == lseek(fd, file_h.offset, SEEK_SET)) {
        perror("lseek error");
        close(fd);
        return -1;
    }

    /* 申请一个buf、暂存bmp图像的一行数据 */
    line_bytes = info_h.width * info_h.bpp / 8;
    line_buf = malloc(line_bytes);
    if (NULL == line_buf) {
        fprintf(stderr, "malloc error\n");
        close(fd);
        return -1;
    }

    if (line_length > line_bytes)
        min_bytes = line_bytes;
    else
        min_bytes = line_length;

    /**** 读取图像数据显示到LCD ****/
    /*******************************************
     * 为了软件处理上方便，这个示例代码便不去做兼容性设计了
     * 如果你想做兼容, 可能需要判断传入的BMP图像是565还是888
     * 如何判断呢？文档里边说的很清楚了
     * 我们默认传入的bmp图像是RGB565格式
     *******************************************/
    if (0 < info_h.height) {//倒向位图
        if (info_h.height > height) {
            min_h = height;
            lseek(fd, (info_h.height - height) * line_bytes, SEEK_CUR);
            screen_base += width * (height - 1);    //定位到屏幕左下角位置
        }
        else {
            min_h = info_h.height;
            screen_base += width * (info_h.height - 1); //定位到....不知怎么描述 懂的人自然懂!
        }

        for (j = min_h; j > 0; screen_base -= width, j--) {
            read(fd, line_buf, line_bytes); //读取出图像数据
            memcpy(screen_base, line_buf, min_bytes);//刷入LCD显存
        }
    }
    else {  //正向位图
        int temp = 0 - info_h.height;   //负数转成正数
        if (temp > height)
            min_h = height;
        else
            min_h = temp;

        for (j = 0; j < min_h; j++, screen_base += width) {
            read(fd, line_buf, line_bytes);
            memcpy(screen_base, line_buf, min_bytes);
        }
    }

    /* 关闭文件、函数返回 */
    close(fd);
    free(line_buf);
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
        fprintf(stderr, "usage: %s <bmp_file>\n", argv[0]);
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

    screen_size = fb_fix.line_length * fb_var.yres;
    line_length = fb_fix.line_length;
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
    memset(screen_base, 0xFF, screen_size);
    show_bmp_image(argv[1]);

    /* 退出 */
    munmap(screen_base, screen_size);  //取消映射
    close(fd);  //关闭文件
    exit(EXIT_SUCCESS);    //退出进程
}
