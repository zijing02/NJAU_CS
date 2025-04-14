/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : pwm.c
 作者 : 邓涛
 版本 : V1.0
 描述 : PWM应用程序示例代码
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

static char pwm_path[100];

static int pwm_config(const char *attr, const char *val)
{
    char file_path[100];
    int len;
    int fd;

    sprintf(file_path, "%s/%s", pwm_path, attr);
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
    if (4 != argc) {
        fprintf(stderr, "usage: %s <id> <period> <duty>\n",
                argv[0]);
        exit(-1);
    }

    /* 打印配置信息 */
    printf("PWM config: id<%s>, period<%s>, duty<%s>\n",
            argv[1], argv[2],
            argv[3]);

    /* 导出pwm */
    sprintf(pwm_path, "/sys/class/pwm/pwmchip%s/pwm0", argv[1]);

    if (access(pwm_path, F_OK)) {//如果pwm0目录不存在, 则导出

        char temp[100];
        int fd;

        sprintf(temp, "/sys/class/pwm/pwmchip%s/export", argv[1]);
        if (0 > (fd = open(temp, O_WRONLY))) {
            perror("open error");
            exit(-1);
        }

        if (1 != write(fd, "0", 1)) {//导出pwm
            perror("write error");
            close(fd);
            exit(-1);
        }

        close(fd);  //关闭文件
    }

    /* 配置PWM周期 */
    if (pwm_config("period", argv[2]))
        exit(-1);

    /* 配置占空比 */
    if (pwm_config("duty_cycle", argv[3]))
        exit(-1);

    /* 使能pwm */
    pwm_config("enable", "1");

    /* 退出程序 */
    exit(0);
}
