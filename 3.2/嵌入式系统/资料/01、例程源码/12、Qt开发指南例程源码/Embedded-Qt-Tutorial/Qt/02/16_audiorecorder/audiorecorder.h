/******************************************************************
Copyright (C) 2017 The Qt Company Ltd.
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   16_audiorecorder
* @brief         audiorecorder.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-10
*******************************************************************/
#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QAudioRecorder>
#include <QAudioProbe>
#include <QAudioBuffer>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QProgressBar>

/* 媒体信息结构体 */
struct MediaObjectInfo {
    /* 用于保存视频文件名 */
    QString fileName;
    /* 用于保存视频文件路径 */
    QString filePath;
};

class AudioRecorder : public QMainWindow
{
    Q_OBJECT

public:
    AudioRecorder(QWidget *parent = nullptr);
    ~AudioRecorder();

private:
    /* 布局初始化 */
    void layoutInit();

    /* 主Widget*/
    QWidget *mainWidget;

    /* 录音列表 */
    QListWidget *listWidget;

    /* 底部的Widget,用于存放按钮 */
    QWidget *bottomWidget;

    /* 中间的显示录制时长的Widget容器 */
    QWidget *centerWidget;

    /* 垂直布局 */
    QVBoxLayout *vBoxLayout;

    /* 录音Level布局 */
    QHBoxLayout *levelHBoxLayout;

    /* 水平布局 */
    QHBoxLayout *hBoxLayout;

    /* 录音按钮 */
    QPushButton *recorderBt;

    /* 上一首按钮 */
    QPushButton *previousBt;

    /* 下一首按钮 */
    QPushButton *nextBt;

    /* 删除按钮 */
    QPushButton *removeBt;

    /* 录音类 */
    QAudioRecorder *m_audioRecorder = nullptr;

    /* 用于探测缓冲区的level */
    QAudioProbe *m_probe = nullptr;

    /* 扫描录音文件 */
    void scanRecordFiles();

    /* 录音设置容器，保存录音设备的可用信息，
     * 本例使用默认的信息，即可录音 */
    QList<QVariant>devicesVar;
    QList<QVariant>codecsVar;
    QList<QVariant>containersVar;
    QList<QVariant>sampleRateVar;
    QList<QVariant>channelsVar;
    QList<QVariant>qualityVar;
    QList<QVariant>bitratesVar;

    /* 媒体播放器，用于播放视频 */
    QMediaPlayer *recorderPlayer;

    /* 媒体列表 */
    QMediaPlaylist *mediaPlaylist;

    /* 录音媒体信息存储 */
    QVector<MediaObjectInfo> mediaObjectInfo;

    /* 用于显示录音时长 */
    QLabel *countLabel;

    /* 用于显示录音level,最多四通道 */
    QProgressBar *progressBar[4];

    /* 清空录音level */
    void clearAudioLevels();

private slots:
    /* 点击录音按钮槽函数 */
    void recorderBtClicked();

    /* 播放列表点击 */
    void listWidgetCliked(QListWidgetItem*);

    /* 当前媒体状态改变 */
    void mediaPlayerStateChanged(QMediaPlayer::State);

    /* 媒体列表改变 */
    void mediaPlaylistCurrentIndexChanged(int);

    /* 当前列表项改变 */
    void listWidgetCurrentItemChange(QListWidgetItem*,
                                     QListWidgetItem*);

    /* 上一首按钮点击 */
    void previousBtClicked();

    /* 下一首按钮点击 */
    void nextBtClicked();

    /* 删除按钮点击 */
    void removeBtClicked();

    /* 更新录音时长 */
    void updateProgress(qint64);

    /* 在列表里显示播放时间 */
    void recorderPlayerPositionChanged(qint64);

    /* 更新录音level */
    void processBuffer(const QAudioBuffer&);
};
#endif // AUDIORECORDER_H
