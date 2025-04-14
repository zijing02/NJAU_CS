/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : pcm_capture_poll.c
 作者 : 邓涛
 版本 : V1.0
 描述 : 一个简单地PCM音频采集示例代码--使用I/O多路复用（poll）读数据
 其他 : 无
 论坛 : www.openedv.com
 日志 : 初版 V1.0 2021/7/20 邓涛创建
 ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <alsa/asoundlib.h>

/************************************
 宏定义
 ************************************/
#define PCM_CAPTURE_DEV    "hw:0,0"

/************************************
 static静态全局变量定义
 ************************************/
static snd_pcm_t *pcm = NULL;               //pcm句柄
static snd_pcm_uframes_t period_size = 1024;    //周期大小（单位: 帧）
static unsigned int periods = 16;               //周期数（buffer的大小）
static unsigned int rate = 44100;               //采样率

static struct pollfd *pfds = NULL;
static int count;

static int snd_pcm_init(void)
{
    snd_pcm_hw_params_t *hwparams = NULL;
    int ret;

    /* 打开PCM设备 */
    ret = snd_pcm_open(&pcm, PCM_CAPTURE_DEV, SND_PCM_STREAM_CAPTURE, 0);
    if (0 > ret) {
        fprintf(stderr, "snd_pcm_open error: %s: %s\n",
                    PCM_CAPTURE_DEV, snd_strerror(ret));
        return -1;
    }

    /* 实例化hwparams对象 */
    snd_pcm_hw_params_malloc(&hwparams);

    /* 获取PCM设备当前硬件配置,对hwparams进行初始化 */
    ret = snd_pcm_hw_params_any(pcm, hwparams);
    if (0 > ret) {
        fprintf(stderr, "snd_pcm_hw_params_any error: %s\n", snd_strerror(ret));
        goto err2;
    }

    /************** 
     设置参数
    ***************/
    /* 设置访问类型: 交错模式 */
    ret = snd_pcm_hw_params_set_access(pcm, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (0 > ret) {
        fprintf(stderr, "snd_pcm_hw_params_set_access error: %s\n", snd_strerror(ret));
        goto err2;
    }

    /* 设置数据格式: 有符号16位、小端模式 */
    ret = snd_pcm_hw_params_set_format(pcm, hwparams, SND_PCM_FORMAT_S16_LE);
    if (0 > ret) {
        fprintf(stderr, "snd_pcm_hw_params_set_format error: %s\n", snd_strerror(ret));
        goto err2;
    }

    /* 设置采样率 */
    ret = snd_pcm_hw_params_set_rate(pcm, hwparams, rate, 0);
    if (0 > ret) {
        fprintf(stderr, "snd_pcm_hw_params_set_rate error: %s\n", snd_strerror(ret));
        goto err2;
    }

    /* 设置声道数: 双声道 */
    ret = snd_pcm_hw_params_set_channels(pcm, hwparams, 2);
    if (0 > ret) {
        fprintf(stderr, "snd_pcm_hw_params_set_channels error: %s\n", snd_strerror(ret));
        goto err2;
    }

    /* 设置周期大小: period_size */
    ret = snd_pcm_hw_params_set_period_size(pcm, hwparams, period_size, 0);
    if (0 > ret) {
        fprintf(stderr, "snd_pcm_hw_params_set_period_size error: %s\n", snd_strerror(ret));
        goto err2;
    }

    /* 设置周期数（buffer的大小）: periods */
    ret = snd_pcm_hw_params_set_periods(pcm, hwparams, periods, 0);
    if (0 > ret) {
        fprintf(stderr, "snd_pcm_hw_params_set_periods error: %s\n", snd_strerror(ret));
        goto err2;
    }

    /* 使配置生效 */
    ret = snd_pcm_hw_params(pcm, hwparams);
    snd_pcm_hw_params_free(hwparams);   //释放hwparams对象占用的内存
    if (0 > ret) {
        fprintf(stderr, "snd_pcm_hw_params error: %s\n", snd_strerror(ret));
        goto err1;
    }

    return 0;

err2:
    snd_pcm_hw_params_free(hwparams);   //释放内存
err1:
    snd_pcm_close(pcm); //关闭pcm设备
    return -1;
}

static int snd_pcm_poll_init(void)
{
    int ret;

    /* 获取PCM句柄的轮询描述符计数 */
    count = snd_pcm_poll_descriptors_count(pcm);
    if (0 >= count) {
        fprintf(stderr, "Invalid poll descriptors count\n");
        return -1;
    }

    /* 分配内存 */
    pfds = calloc(count, sizeof(struct pollfd));
    if (NULL == pfds) {
        perror("calloc error");
        return -1;
    }

    /* 填充pfds */
    ret = snd_pcm_poll_descriptors(pcm, pfds, count);
    if (0 > ret)
        return -1;

    return 0;
}

/************************************
 main主函数
 ************************************/
int main(int argc, char *argv[])
{
    unsigned char *buf = NULL;
    unsigned int buf_bytes;
    unsigned short revents;
    snd_pcm_sframes_t avail;
    int fd = -1;
    int ret;

    if (2 != argc) {
        fprintf(stderr, "Usage: %s <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* 初始化PCM Capture设备 */
    if (snd_pcm_init())
        exit(EXIT_FAILURE);

    /* 申请读缓冲区 */
    buf_bytes = period_size * 4;    //字节大小 = 周期大小*帧的字节大小 16位双声道
    buf = malloc(buf_bytes);
    if (NULL == buf) {
        perror("malloc error");
        goto err1;
    }

    /* 打开一个新建文件 */
    fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL);
    if (0 > fd) {
        fprintf(stderr, "open error: %s: %s\n", argv[1], strerror(errno));
        goto err2;
    }

    /* I/O多路复用poll初始化 */
    if (snd_pcm_poll_init())
        goto err3;

    /* 开始录音 */
    ret = snd_pcm_start(pcm);
    if (0 > ret) {
        fprintf(stderr, "snd_pcm_start error: %s\n", snd_strerror(ret));
        goto err3;
    }

    for (;;) {

        ret = poll(pfds, count, -1);//调用poll
        if (0 > ret) {
            perror("poll error");
            goto err3;
        }

        ret = snd_pcm_poll_descriptors_revents(pcm, pfds, count, &revents);
        if (0 > ret)
            goto err3;
        if (revents & POLLERR)
            goto err3;
        if (revents & POLLIN) { //可读数据
            avail = snd_pcm_avail_update(pcm);   //检查有多少帧数据可读
            while (avail >= period_size) {  //每次读取一个周期

                ret = snd_pcm_readi(pcm, buf, period_size);//读取PCM数据 一个周期
                if (0 > ret) {
                    fprintf(stderr, "snd_pcm_readi error: %s\n", snd_strerror(ret));
                    goto err3;
                }

                ret = write(fd, buf, ret * 4);    //将读取到的数据写入文件中
                if (0 >= ret)
                    goto err3;

                avail = snd_pcm_avail_update(pcm);   //再次读取、更新avail
            }
        }
    }

err3:
    close(fd);  //关闭文件
err2:
    free(buf);     //释放内存
err1:
    snd_pcm_close(pcm); //关闭pcm设备
    exit(EXIT_FAILURE);
}
