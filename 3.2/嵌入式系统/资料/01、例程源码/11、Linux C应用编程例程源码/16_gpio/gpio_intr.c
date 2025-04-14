/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : pwm.c
 作者 : 邓涛
 版本 : V1.0
 描述 : GPIO应用编程示例代码--中断
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
#include <poll.h>

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
        return -1;
    }

    close(fd);  //关闭文件
    return 0;
}

int main(int argc, char *argv[])
{
    struct pollfd pfd;
    char file_path[100];
    int ret;
    char val;

    /* 校验传参 */
    if (2 != argc) {
        fprintf(stderr, "usage: %s <gpio>\n", argv[0]);
        exit(-1);
    }

    /* 判断指定编号的GPIO是否导出 */
    sprintf(gpio_path, "/sys/class/gpio/gpio%s", argv[1]);

    if (access(gpio_path, F_OK)) {//如果目录不存在 则需要导出

        int len;
        int fd;

        if (0 > (fd = open("/sys/class/gpio/export", O_WRONLY))) {
            perror("open error");
            exit(-1);
        }

        len = strlen(argv[1]);
        if (len != write(fd, argv[1], len)) {//导出gpio
            perror("write error");
            exit(-1);
        }

        close(fd);  //关闭文件
    }

    /* 配置为输入模式 */
    if (gpio_config("direction", "in"))
        exit(-1);

    /* 极性设置 */
    if (gpio_config("active_low", "0"))
        exit(-1);

    /* 配置中断触发方式: 上升沿和下降沿 */
    if (gpio_config("edge", "both"))
        exit(-1);

    /* 打开value属性文件 */
    sprintf(file_path, "%s/%s", gpio_path, "value");

    if (0 > (pfd.fd = open(file_path, O_RDONLY))) {
        perror("open error");
        exit(-1);
    }

    /* 调用poll */
    pfd.events = POLLPRI; //只关心高优先级数据可读（中断）

    read(pfd.fd, &val, 1);//先读取一次清除状态
    for ( ; ; ) {

        ret = poll(&pfd, 1, -1);    //调用poll
        if (0 > ret) {
            perror("poll error");
            exit(-1);
        }
        else if (0 == ret) {
            fprintf(stderr, "poll timeout.\n");
            continue;
        }

        /* 校验高优先级数据是否可读 */
        if(pfd.revents & POLLPRI) {
            if (0 > lseek(pfd.fd, 0, SEEK_SET)) {//将读位置移动到头部
                perror("lseek error");
                exit(-1);
            }

            if (0 > read(pfd.fd, &val, 1)) {
                perror("read error");
                exit(-1);
            }

            printf("GPIO中断触发<value=%c>\n", val);
        }
    }

    /* 退出程序 */
    exit(0);
}
