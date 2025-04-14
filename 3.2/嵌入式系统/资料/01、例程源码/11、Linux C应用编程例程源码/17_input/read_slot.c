#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

int main(int argc, char *argv[])
{
    struct input_absinfo info;
    int fd = -1;
    int max_slots;

    /* 校验传参 */
    if (2 != argc) {
        fprintf(stderr, "usage: %s <input-dev>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* 打开文件 */
    if (0 > (fd = open(argv[1], O_RDONLY))) {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    /* 获取slot信息 */
    if (0 > ioctl(fd, EVIOCGABS(ABS_MT_SLOT), &info)) {
        perror("ioctl error");
        close(fd);
        exit(EXIT_FAILURE);
    }

    max_slots = info.maximum + 1 - info.minimum;
    printf("max_slots: %d\n", max_slots);

    /* 关闭、退出 */
    close(fd);
    exit(EXIT_SUCCESS);
}
