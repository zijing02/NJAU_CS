#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

int main(int argc, char *argv[])
{
    struct fb_fix_screeninfo fb_fix;
    struct fb_var_screeninfo fb_var;
    int fd;

    /* 打开framebuffer设备 */
    if (0 > (fd = open("/dev/fb0", O_WRONLY))) {
        perror("open error");
        exit(-1);
    }

    /* 获取参数信息 */
    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);
    printf("分辨率: %d*%d\n"
        "像素深度bpp: %d\n"
        "一行的字节数: %d\n"
        "像素格式: R<%d %d> G<%d %d> B<%d %d>\n",
        fb_var.xres, fb_var.yres, fb_var.bits_per_pixel,
        fb_fix.line_length,
        fb_var.red.offset, fb_var.red.length,
        fb_var.green.offset, fb_var.green.length,
        fb_var.blue.offset, fb_var.blue.length);

    /* 关闭设备文件退出程序 */
    close(fd);
    exit(0);
}
