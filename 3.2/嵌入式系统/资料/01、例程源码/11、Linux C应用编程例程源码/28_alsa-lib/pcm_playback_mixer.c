/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : pcm_playback_mixer.c
 作者 : 邓涛
 版本 : V1.0
 描述 : 一个简单地PCM播放示例代码--使用异步方式、加入混音器设置
 其他 : 无
 论坛 : www.openedv.com
 日志 : 初版 V1.0 2021/7/20 邓涛创建
 ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <signal.h>
#include <alsa/asoundlib.h>

/************************************
 宏定义
 ************************************/
#define PCM_PLAYBACK_DEV    "hw:0,0"
#define MIXER_DEV           "hw:0"

/************************************
 WAV音频文件解析相关数据结构申明
 ************************************/
typedef struct WAV_RIFF {
    char ChunkID[4];                	/* "RIFF" */
    u_int32_t ChunkSize;            	/* 从下一个地址开始到文件末尾的总字节数 */
    char Format[4];                 	/* "WAVE" */
} __attribute__ ((packed)) RIFF_t;

typedef struct WAV_FMT {
    char Subchunk1ID[4];             	/* "fmt " */
    u_int32_t Subchunk1Size;          	/* 16 for PCM */
    u_int16_t AudioFormat;            	/* PCM = 1*/
    u_int16_t NumChannels;           	/* Mono = 1, Stereo = 2, etc. */
    u_int32_t SampleRate;             	/* 8000, 44100, etc. */
    u_int32_t ByteRate;               	/* = SampleRate * NumChannels * BitsPerSample/8 */
    u_int16_t BlockAlign;             	/* = NumChannels * BitsPerSample/8 */
    u_int16_t BitsPerSample;          	/* 8bits, 16bits, etc. */
} __attribute__ ((packed)) FMT_t;
static FMT_t wav_fmt;

typedef struct WAV_DATA {
    char Subchunk2ID[4];             	/* "data" */
    u_int32_t Subchunk2Size;          	/* data size */
} __attribute__ ((packed)) DATA_t;

/************************************
 static静态全局变量定义
 ************************************/
static snd_pcm_t *pcm = NULL;                   	//pcm句柄
static snd_mixer_t *mixer = NULL;                  	//混音器句柄
static snd_mixer_elem_t *playback_vol_elem = NULL; 	//播放<音量控制>元素
static unsigned int buf_bytes;                      	//应用程序缓冲区的大小（字节为单位）
static void *buf = NULL;                        	//指向应用程序缓冲区的指针
static int fd = -1;                                 	//指向WAV音频文件的文件描述符
static snd_pcm_uframes_t period_size = 1024;        	//周期大小（单位: 帧）
static unsigned int periods = 16;                   	//周期数（设备驱动层buffer的大小）
static struct termios old_cfg;                      	//用于保存终端当前的配置参数

/************************************
 static静态函数
 ************************************/
static void snd_playback_async_callback(snd_async_handler_t *handler)
{
    snd_pcm_t *handle = snd_async_handler_get_pcm(handler);//获取PCM句柄
    snd_pcm_sframes_t avail;
    int ret;

    avail = snd_pcm_avail_update(handle);//获取环形缓冲区中有多少帧数据需要填充
    while (avail >= period_size) {  //我们一次写入一个周期

        memset(buf, 0x00, buf_bytes);   //buf清零
        ret = read(fd, buf, buf_bytes);
        if (0 >= ret)
            goto out;

        ret = snd_pcm_writei(handle, buf, period_size);
        if (0 > ret) {
            fprintf(stderr, "snd_pcm_writei error: %s\n", snd_strerror(ret));
            goto out;
        }
        else if (ret < period_size) {//实际写入的帧数小于指定的帧数
            //此时我们需要调整下音频文件的读位置
            //将读位置向后移动（往回移）(period_size-ret)*frame_bytes个字节
            //frame_bytes表示一帧的字节大小
            if (0 > lseek(fd, (ret-period_size) * wav_fmt.BlockAlign, SEEK_CUR)) {
                perror("lseek error");
                goto out;
            }
        }

        avail = snd_pcm_avail_update(handle);   //再次获取、更新avail
    }

    return;
out:
    snd_pcm_drain(pcm); //停止PCM
    snd_mixer_close(mixer); //关闭混音器
    snd_pcm_close(handle); //关闭pcm设备
    tcsetattr(STDIN_FILENO, TCSANOW, &old_cfg); //退出前恢复终端的状态
    free(buf);
    close(fd);      //关闭打开的音频文件
    exit(EXIT_FAILURE); //退出程序
}

static int snd_pcm_init(void)
{
    snd_pcm_hw_params_t *hwparams = NULL;
    snd_async_handler_t *async_handler = NULL;
    int ret;

    /* 打开PCM设备 */
    ret = snd_pcm_open(&pcm, PCM_PLAYBACK_DEV, SND_PCM_STREAM_PLAYBACK, 0);
    if (0 > ret) {
        fprintf(stderr, "snd_pcm_open error: %s: %s\n",
                    PCM_PLAYBACK_DEV, snd_strerror(ret));
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
    ret = snd_pcm_hw_params_set_rate(pcm, hwparams, wav_fmt.SampleRate, 0);
    if (0 > ret) {
        fprintf(stderr, "snd_pcm_hw_params_set_rate error: %s\n", snd_strerror(ret));
        goto err2;
    }

    /* 设置声道数: 双声道 */
    ret = snd_pcm_hw_params_set_channels(pcm, hwparams, wav_fmt.NumChannels);
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

    /* 设置周期数（驱动层环形缓冲区buffer的大小）: periods */
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

    buf_bytes = period_size * wav_fmt.BlockAlign; //变量赋值，一个周期的字节大小

    /* 注册异步处理函数 */
    ret = snd_async_add_pcm_handler(&async_handler, pcm, snd_playback_async_callback, NULL);
    if (0 > ret) {
        fprintf(stderr, "snd_async_add_pcm_handler error: %s\n", snd_strerror(ret));
        goto err1;
    }

    return 0;

err2:
    snd_pcm_hw_params_free(hwparams);   //释放内存
err1:
    snd_pcm_close(pcm); //关闭pcm设备
    return -1;
}

static int snd_mixer_init(void)
{
    snd_mixer_elem_t *elem = NULL;
    const char *elem_name;
    long minvol, maxvol;
    int ret;

    /* 打开混音器 */
    ret = snd_mixer_open(&mixer, 0);
    if (0 > ret) {
        fprintf(stderr, "snd_mixer_open error: %s\n", snd_strerror(ret));
        return -1;
    }

    /* 关联一个声卡控制设备 */
    ret = snd_mixer_attach(mixer, MIXER_DEV);
    if (0 > ret) {
        fprintf(stderr, "snd_mixer_attach error: %s\n", snd_strerror(ret));
        goto err;
    }

    /* 注册混音器 */
    ret = snd_mixer_selem_register(mixer, NULL, NULL);
    if (0 > ret) {
        fprintf(stderr, "snd_mixer_selem_register error: %s\n", snd_strerror(ret));
        goto err;
    }

    /* 加载混音器 */
    ret = snd_mixer_load(mixer);
    if (0 > ret) {
        fprintf(stderr, "snd_mixer_load error: %s\n", snd_strerror(ret));
        goto err;
    }

    /* 遍历混音器中的元素 */
    elem = snd_mixer_first_elem(mixer);//找到第一个元素
    while (elem) {

        elem_name = snd_mixer_selem_get_name(elem);//获取元素的名称
        /* 针对开发板出厂系统：WM8960声卡设备 */
        if(!strcmp("Speaker", elem_name) ||  //耳机音量<对喇叭外音输出有效>
           !strcmp("Headphone", elem_name) ||//喇叭音量<对耳机输出有效>
           !strcmp("Playback", elem_name)) {//播放音量<总的音量控制，对喇叭和耳机输出都有效>
            if (snd_mixer_selem_has_playback_volume(elem)) {//是否是音量控制元素
                snd_mixer_selem_get_playback_volume_range(elem, &minvol, &maxvol);//获取音量可设置范围
                snd_mixer_selem_set_playback_volume_all(elem, (maxvol-minvol)*0.9 + minvol);//全部设置为90%

                if (!strcmp("Playback", elem_name))
                    playback_vol_elem = elem;
            }
        }

        elem = snd_mixer_elem_next(elem);
    }

    return 0;

err:
    snd_mixer_close(mixer);
    return -1;
}

static int open_wav_file(const char *file)
{
    RIFF_t wav_riff;
    DATA_t wav_data;
    int ret;

    fd = open(file, O_RDONLY);
    if (0 > fd) {
        fprintf(stderr, "open error: %s: %s\n", file, strerror(errno));
        return -1;
    }

    /* 读取RIFF chunk */
    ret = read(fd, &wav_riff, sizeof(RIFF_t));
    if (sizeof(RIFF_t) != ret) {
        if (0 > ret)
            perror("read error");
        else
            fprintf(stderr, "check error: %s\n", file);
        close(fd);
        return -1;
    }

    if (strncmp("RIFF", wav_riff.ChunkID, 4) ||//校验
        strncmp("WAVE", wav_riff.Format, 4)) {
        fprintf(stderr, "check error: %s\n", file);
        close(fd);
        return -1;
    }

    /* 读取sub-chunk-fmt */
    ret = read(fd, &wav_fmt, sizeof(FMT_t));
    if (sizeof(FMT_t) != ret) {
        if (0 > ret)
            perror("read error");
        else
            fprintf(stderr, "check error: %s\n", file);
        close(fd);
        return -1;
    }

    if (strncmp("fmt ", wav_fmt.Subchunk1ID, 4)) {//校验
        fprintf(stderr, "check error: %s\n", file);
        close(fd);
        return -1;
    }

    /* 打印音频文件的信息 */
    printf("<<<<音频文件格式信息>>>>\n\n");
    printf("  file name:     %s\n", file);
    printf("  Subchunk1Size: %u\n", wav_fmt.Subchunk1Size);
    printf("  AudioFormat:   %u\n", wav_fmt.AudioFormat);
    printf("  NumChannels:   %u\n", wav_fmt.NumChannels);
    printf("  SampleRate:    %u\n", wav_fmt.SampleRate);
    printf("  ByteRate:      %u\n", wav_fmt.ByteRate);
    printf("  BlockAlign:    %u\n", wav_fmt.BlockAlign);
    printf("  BitsPerSample: %u\n\n", wav_fmt.BitsPerSample);

    /* sub-chunk-data */
    if (0 > lseek(fd, sizeof(RIFF_t) + 8 + wav_fmt.Subchunk1Size,
                SEEK_SET)) {
        perror("lseek error");
        close(fd);
        return -1;
    }

    while(sizeof(DATA_t) == read(fd, &wav_data, sizeof(DATA_t))) {

        /* 找到sub-chunk-data */
        if (!strncmp("data", wav_data.Subchunk2ID, 4))//校验
            return 0;

        if (0 > lseek(fd, wav_data.Subchunk2Size, SEEK_CUR)) {
            perror("lseek error");
            close(fd);
            return -1;
        }
    }

    fprintf(stderr, "check error: %s\n", file);
    return -1;
}

static void show_help(void)
{
    printf("<<<<<<<基于alsa-lib音乐播放器>>>>>>>>>\n\n"
           "操作菜单:\n"
           "  q             退出程序\n"
           "  space<空格>   暂停播放/恢复播放\n"
           "  w             音量增加++\n"
           "  s             音量减小--\n\n");
}

/************************************
 main主函数
 ************************************/
int main(int argc, char *argv[])
{
    snd_pcm_sframes_t avail;
    struct termios new_cfg;
    sigset_t sset;
    int ret;

    if (2 != argc) {
        fprintf(stderr, "Usage: %s <audio_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* 屏蔽SIGIO信号 */
    sigemptyset(&sset);
    sigaddset(&sset, SIGIO);
    sigprocmask(SIG_BLOCK, &sset, NULL);

    /* 打开WAV音频文件 */
    if (open_wav_file(argv[1]))
        exit(EXIT_FAILURE);

    /* 初始化PCM Playback设备 */
    if (snd_pcm_init())
        goto err1;

    /* 初始化混音器 */
    if (snd_mixer_init())
        goto err2;

    /* 申请读缓冲区 */
    buf = malloc(buf_bytes);
    if (NULL == buf) {
        perror("malloc error");
        goto err3;
    }

    /* 终端配置 */
    tcgetattr(STDIN_FILENO, &old_cfg);  //获取终端<标准输入-标准输出构成了一套终端>
    memcpy(&new_cfg, &old_cfg, sizeof(struct termios));//备份
    new_cfg.c_lflag &= ~ICANON; //将终端设置为非规范模式
    new_cfg.c_lflag &= ~ECHO;   //禁用回显
    tcsetattr(STDIN_FILENO, TCSANOW, &new_cfg);//使配置生效

    /* 播放：先将环形缓冲区填满数据 */
    avail = snd_pcm_avail_update(pcm);  //获取环形缓冲区中有多少帧数据需要填充
    while (avail >= period_size) {  //我们一次写入一个周期

        memset(buf, 0x00, buf_bytes);   //buf清零
        ret = read(fd, buf, buf_bytes);
        if (0 >= ret)
            goto err4;

        ret = snd_pcm_writei(pcm, buf, period_size);//向环形缓冲区中写入数据
        if (0 > ret) {
            fprintf(stderr, "snd_pcm_writei error: %s\n", snd_strerror(ret));
            goto err4;
        }
        else if (ret < period_size) {//实际写入的帧数小于指定的帧数
            //此时我们需要调整下音频文件的读位置
            //将读位置向后移动（往回移）(period_size-ret)*frame_bytes个字节
            //frame_bytes表示一帧的字节大小
            if (0 > lseek(fd, (ret-period_size) * wav_fmt.BlockAlign, SEEK_CUR)) {
                perror("lseek error");
                goto err4;
            }
        }

        avail = snd_pcm_avail_update(pcm);  //再次获取、更新avail
    }

    sigprocmask(SIG_UNBLOCK, &sset, NULL);  //取消SIGIO信号屏蔽

    /* 显示帮助信息 */
    show_help();

    /* 等待获取用户输入 */
    char ch;
    long vol;
    for ( ; ; ) {

        ch = getchar(); //获取用户输入的控制字符
        switch (ch) {
        case 'q':   //Q键退出程序
            sigprocmask(SIG_BLOCK, &sset, NULL);//屏蔽SIGIO信号
            goto err4;
        case ' ':   //空格暂停/恢复
            switch (snd_pcm_state(pcm)) {

            case SND_PCM_STATE_PAUSED:  //如果是暂停状态则恢复运行
                ret = snd_pcm_pause(pcm, 0);
                if (0 > ret)
                    fprintf(stderr, "snd_pcm_pause error: %s\n", snd_strerror(ret));
                break;
            case SND_PCM_STATE_RUNNING: //如果是运行状态则暂停
                ret = snd_pcm_pause(pcm, 1);
                if (0 > ret)
                    fprintf(stderr, "snd_pcm_pause error: %s\n", snd_strerror(ret));
                break;
            }
            break;
        case 'w':           //音量增加
            if (playback_vol_elem) {
                //获取音量
                snd_mixer_selem_get_playback_volume(playback_vol_elem,
                        SND_MIXER_SCHN_FRONT_LEFT, &vol);
                vol++;
                //设置音量
                snd_mixer_selem_set_playback_volume_all(playback_vol_elem, vol);
            }
            break;
        case 's':           //音量降低
            if (playback_vol_elem) {
                //获取音量
                snd_mixer_selem_get_playback_volume(playback_vol_elem,
                        SND_MIXER_SCHN_FRONT_LEFT, &vol);
                vol--;
                //设置音量
                snd_mixer_selem_set_playback_volume_all(playback_vol_elem, vol);
            }
            break;
        }
    }

err4:
    snd_pcm_drop(pcm); //停止PCM
    tcsetattr(STDIN_FILENO, TCSANOW, &old_cfg); //退出前恢复终端的状态
    free(buf);     //释放内存
err3:
    snd_mixer_close(mixer); //关闭混音器
err2:
    snd_pcm_close(pcm); //关闭pcm设备
err1:
    close(fd);      //关闭打开的音频文件
    exit(EXIT_FAILURE);
}
