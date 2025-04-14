#include <stdio.h>
#include <stdlib.h>
#include <tslib.h>      //包含tslib.h头文件

int main(int argc, char *argv[])
{
    struct tsdev *ts = NULL;
    struct ts_sample samp;
    int pressure = 0;//用于保存上一次的按压力,初始为0,表示松开

    /* 打开并配置触摸屏设备 */
    ts = ts_setup(NULL, 0);
    if (NULL == ts) {
        fprintf(stderr, "ts_setup error");
        exit(EXIT_FAILURE);
    }

    /* 读数据 */
    for ( ; ; ) {

        if (0 > ts_read(ts, &samp, 1)) {
            fprintf(stderr, "ts_read error");
            ts_close(ts);
            exit(EXIT_FAILURE);
        }

        if (samp.pressure) {//按压力>0
            if (pressure)   //若上一次的按压力>0
                printf("移动(%d, %d)\n", samp.x, samp.y);
            else
                printf("按下(%d, %d)\n", samp.x, samp.y);
        }
        else
            printf("松开\n");//打印坐标

        pressure = samp.pressure;
    }

    ts_close(ts);
    exit(EXIT_SUCCESS);
}
