/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : read_mt.c
 作者 : 邓涛
 版本 : V1.0
 描述 : 触摸屏多点触摸应用程序示例代码
 其他 : 无
 论坛 : www.openedv.com
 日志 : 初版 V1.0 2021/6/15 邓涛创建
 ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <linux/input.h>

/* 用于描述MT多点触摸每一个触摸点的信息 */
struct ts_mt {
    int x;      	//X坐标
    int y;      	//Y坐标
    int id;     	//对应ABS_MT_TRACKING_ID
    int valid;		//数据有效标志位(=1表示触摸点信息发生更新)
};

/* 一个触摸点的x坐标和y坐标 */
struct tp_xy {
    int x;
    int y;
};

static int ts_read(const int fd, const int max_slots,
        struct ts_mt *mt)
{
    struct input_event in_ev;
    static int slot = 0;//用于保存上一个slot
    static struct tp_xy xy[12] = {0};//用于保存上一次的x和y坐标值,假设触摸屏支持的最大触摸点数不会超过12
    int i;

    /* 对缓冲区初始化操作 */
    memset(mt, 0x0, max_slots * sizeof(struct ts_mt));  //清零
    for (i = 0; i < max_slots; i++)
        mt[i].id = -2;//将id初始化为-2, id=-1表示触摸点删除, id>=0表示创建

    for ( ; ; ) {

        if (sizeof(struct input_event) !=
            read(fd, &in_ev, sizeof(struct input_event))) {
            perror("read error");
            return -1;
        }

        switch (in_ev.type) {
        case EV_ABS:
            switch (in_ev.code) {
            case ABS_MT_SLOT:
                slot = in_ev.value;
                break;
            case ABS_MT_POSITION_X:
                xy[slot].x = in_ev.value;
                mt[slot].valid = 1;
                break;
            case ABS_MT_POSITION_Y:
                xy[slot].y = in_ev.value;
                mt[slot].valid = 1;
                break;
            case ABS_MT_TRACKING_ID:
                mt[slot].id = in_ev.value;
                mt[slot].valid = 1;
                break;
            }
            break;
        //case EV_KEY://按键事件对单点触摸应用比较有用
        //  break;
        case EV_SYN:
            if (SYN_REPORT == in_ev.code) {
                for (i = 0; i < max_slots; i++) {
                    mt[i].x = xy[i].x;
                    mt[i].y = xy[i].y;
                }
            }
            return 0;
        }
    }
}

int main(int argc, char *argv[])
{
    struct input_absinfo slot;
    struct ts_mt *mt = NULL;
    int max_slots;
    int fd;
    int i;

    /* 参数校验 */
    if (2 != argc) {
        fprintf(stderr,"usage: %s <input_dev>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* 打开文件 */
    fd = open(argv[1], O_RDONLY);
    if (0 > fd) {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    /* 获取触摸屏支持的最大触摸点数 */
    if (0 > ioctl(fd, EVIOCGABS(ABS_MT_SLOT), &slot)) {
        perror("ioctl error");
        close(fd);
        exit(EXIT_FAILURE);
    }

    max_slots = slot.maximum + 1 - slot.minimum;
    printf("max_slots: %d\n", max_slots);

    /* 申请内存空间并清零 */
    mt = calloc(max_slots, sizeof(struct ts_mt));

    /* 读数据 */
    for ( ; ; ) {

        if (0 > ts_read(fd, max_slots, mt))
            break;

        for (i = 0; i < max_slots; i++) {
            if (mt[i].valid) {//判断每一个触摸点信息是否发生更新（关注的信息发生更新）

                if (0 <= mt[i].id)
                    printf("slot<%d>, 按下(%d, %d)\n", i, mt[i].x, mt[i].y);
                else if (-1 == mt[i].id)
                    printf("slot<%d>, 松开\n", i);
                else
                    printf("slot<%d>, 移动(%d, %d)\n", i, mt[i].x, mt[i].y);
            }
        }
    }

    /* 关闭设备、退出 */
    close(fd);
    free(mt);
    exit(EXIT_FAILURE);
}
