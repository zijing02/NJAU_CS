#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

int main(int argc, char *argv[])
{
    struct input_event in_ev;
    int x, y;   //触摸点x和y坐标
    int down;   //用于记录BTN_TOUCH事件的value,1表示按下,0表示松开,-1表示移动
    int valid;  //用于记录数据是否有效(我们关注的信息发生更新表示有效,1表示有效,0表示无效)
    int fd = -1;

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

    x = y = 0;  //初始化x和y坐标值
    down = -1; //初始化<移动>
    valid = 0;//初始化<无效>
    for ( ; ; ) {

        /* 循环读取数据 */
        if (sizeof(struct input_event) !=
            read(fd, &in_ev, sizeof(struct input_event))) {
            perror("read error");
            exit(EXIT_FAILURE);
        }

        switch (in_ev.type) {
        case EV_KEY:    //按键事件
            if (BTN_TOUCH == in_ev.code) {
                down = in_ev.value;
                valid = 1;
            }
            break;

        case EV_ABS:    //绝对位移事件
            switch (in_ev.code) {
            case ABS_X: //X坐标
                x = in_ev.value;
                valid = 1;
                break;
            case ABS_Y: //Y坐标
                y = in_ev.value;
                valid = 1;
                break;
            }
            break;

        case EV_SYN:    //同步事件
            if (SYN_REPORT == in_ev.code) {
                if (valid) {//判断是否有效
                    switch (down) {//判断状态
                    case 1:
                        printf("按下(%d, %d)\n", x, y);
                        break;
                    case 0:
                        printf("松开\n");
                        break;
                    case -1:
                        printf("移动(%d, %d)\n", x, y);
                        break;
                    }

                    valid = 0; //重置valid
                    down = -1; //重置down
                }
            }
            break;
        }
    }
}
