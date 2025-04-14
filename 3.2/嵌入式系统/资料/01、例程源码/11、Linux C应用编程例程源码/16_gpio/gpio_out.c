/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : pwm.c
 作者 : 邓涛
 版本 : V1.0
 描述 : GPIO应用编程示例代码--输出
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
#include <string.h>

static char gpio_path[100];

static int gpio_config(const char *attr, const char *val)
{
    char file_path[100];
    int len;
    int fd;

    sprintf(file_path, "%s/%s", gpio_path, attr);
    if (0 > (fd = open(file_path, O_WRONLY))) {
        perror("open error");
        return fd;
    }

    len = strlen(val);
    if (len != write(fd, val, len)) {
        perror("write error");
        close(fd);
        return -1;
    }

    close(fd);  //关闭文件
    return 0;
}

int main(int argc, char *argv[])
{
    /* 校验传参 */
    if (3 != argc) {
        fprintf(stderr, "usage: %s <gpio> <value>\n", argv[0]);
        exit(-1);
    }

    /* 判断指定编号的GPIO是否导出 */
    sprintf(gpio_path, "/sys/class/gpio/gpio%s", argv[1]);

    if (access(gpio_path, F_OK)) {//如果目录不存在 则需要导出

        int fd;
        int len;

        if (0 > (fd = open("/sys/class/gpio/export", O_WRONLY))) {
            perror("open error");
            exit(-1);
        }

        len = strlen(argv[1]);
        if (len != write(fd, argv[1], len)) {//导出gpio
            perror("write error");
            close(fd);
            exit(-1);
        }

        close(fd);  //关闭文件
    }

    /* 配置为输出模式 */
    if (gpio_config("direction", "out"))
        exit(-1);

    /* 极性设置 */
    if (gpio_config("active_low", "0"))
        exit(-1);

    /* 控制GPIO输出高低电平 */
    if (gpio_config("value", argv[2]))
        exit(-1);

    /* 退出程序 */
    exit(0);
}

