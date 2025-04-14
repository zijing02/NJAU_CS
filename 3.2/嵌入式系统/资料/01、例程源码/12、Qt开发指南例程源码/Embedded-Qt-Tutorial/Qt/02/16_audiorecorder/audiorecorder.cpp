/******************************************************************
Copyright (C) 2017 The Qt Company Ltd.
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   16_audiorecorder
* @brief         audiorecorder.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-10
*******************************************************************/
#include "audiorecorder.h"
#include <QDebug>
#include <QUrl>
#include <QDateTime>
#include <QDir>
#include <QCoreApplication>

static qreal getPeakValue(const QAudioFormat &format);
static QVector<qreal> getBufferLevels(const QAudioBuffer &buffer);

template <class T>
static QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels);

AudioRecorder::AudioRecorder(QWidget *parent)
    : QMainWindow(parent)
{
    /* 初始化布局 */
    layoutInit();

    /* 录制音频的类 */
    m_audioRecorder = new QAudioRecorder(this);

    /* 用于探测缓冲区的数据 */
    m_probe = new QAudioProbe(this);

    /* 信号槽连接，更新录音level显示 */
    connect(m_probe, &QAudioProbe::audioBufferProbed,
            this, &AudioRecorder::processBuffer);

    /* 设置探测的对象 */
    m_probe->setSource(m_audioRecorder);

    /* 播放器 */
    recorderPlayer = new QMediaPlayer(this);

    /* 播放列表 */
    mediaPlaylist = new QMediaPlaylist(this);

    recorderPlayer->setPlaylist(mediaPlaylist);

    /* 设置播放模式，默认是列表播放 */
    mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);

    /* 扫描本地声卡设备 */
    devicesVar.append(QVariant(QString()));
    for (auto &device: m_audioRecorder->audioInputs()) {
        devicesVar.append(QVariant(device));
        //qDebug()<<"本地声卡设备："<<device<<endl;
    }

    /* 音频编码 */
    codecsVar.append(QVariant(QString()));
    for (auto &codecName: m_audioRecorder->supportedAudioCodecs()) {
        codecsVar.append(QVariant(codecName));
        //qDebug()<<"音频编码："<<codecName<<endl;
    }

    /* 容器/支持的格式 */
    containersVar.append(QVariant(QString()));
    for (auto &containerName: m_audioRecorder->supportedContainers()) {
        containersVar.append(QVariant(containerName));
        //qDebug()<<"支持的格式："<<containerName<<endl;
    }

    /* 采样率 */
    sampleRateVar.append(QVariant(0));
    for (int sampleRate: m_audioRecorder->supportedAudioSampleRates()) {
        sampleRateVar.append(QVariant(sampleRate));
        //qDebug()<<"采样率："<<sampleRate<<endl;
    }

    /* 通道 */
    channelsVar.append(QVariant(-1));
    channelsVar.append(QVariant(1));
    channelsVar.append(QVariant(2));
    channelsVar.append(QVariant(4));

    /* 质量 */
    qualityVar.append(QVariant(int(QMultimedia::LowQuality)));
    qualityVar.append(QVariant(int(QMultimedia::NormalQuality)));
    qualityVar.append(QVariant(int(QMultimedia::HighQuality)));

    /* 比特率 */
    bitratesVar.append(QVariant(0));
    bitratesVar.append(QVariant(32000));
    bitratesVar.append(QVariant(64000));
    bitratesVar.append(QVariant(96000));
    bitratesVar.append(QVariant(128000));

    /* 初始化时扫描已经录制的录音mp3文件 */
    scanRecordFiles();

    /* 录音类信号槽连接 */
    connect(m_audioRecorder, &QAudioRecorder::durationChanged,
            this, &AudioRecorder::updateProgress);

    /* 列表信号槽连接 */
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(listWidgetCliked(QListWidgetItem*)));
    connect(listWidget, SIGNAL(currentItemChanged(QListWidgetItem*,
                                                  QListWidgetItem*)),
            this, SLOT(listWidgetCurrentItemChange(QListWidgetItem*,
                                                   QListWidgetItem*)));

    /* 媒体连接信号槽 */
    connect(recorderPlayer,
            SIGNAL(stateChanged(QMediaPlayer::State)),
            this,
            SLOT(mediaPlayerStateChanged(QMediaPlayer::State)));
    connect(mediaPlaylist,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(mediaPlaylistCurrentIndexChanged(int)));
    connect(recorderPlayer, SIGNAL(positionChanged(qint64)),
            this,
            SLOT(recorderPlayerPositionChanged(qint64)));

    /* 按钮 */
    connect(recorderBt, SIGNAL(clicked()), this, SLOT(recorderBtClicked()));
    connect(nextBt, SIGNAL(clicked()), this, SLOT(nextBtClicked()));
    connect(previousBt, SIGNAL(clicked()), this, SLOT(previousBtClicked()));
    connect(removeBt, SIGNAL(clicked()), this, SLOT(removeBtClicked()));
}

AudioRecorder::~AudioRecorder()
{
}

void AudioRecorder::layoutInit()
{
    this->setGeometry(0, 0, 800, 480);

    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    vBoxLayout = new QVBoxLayout();
    bottomWidget = new QWidget();
    listWidget = new QListWidget();
    listWidget->setFocusPolicy(Qt::NoFocus);
    listWidget->setVerticalScrollBarPolicy(
                Qt::ScrollBarAlwaysOff);
    listWidget->setHorizontalScrollBarPolicy(
                Qt::ScrollBarAlwaysOff);

    /* 垂直布局 */
    vBoxLayout->addWidget(listWidget);
    vBoxLayout->addWidget(bottomWidget);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    mainWidget->setLayout(vBoxLayout);

    bottomWidget->setMinimumHeight(80);
    bottomWidget->setMaximumHeight(80);
    bottomWidget->setStyleSheet("background:#cccccc");

    /* 水平布局 */
    hBoxLayout = new QHBoxLayout();

    /* 按钮，录音、上一首、下一首、删除项按钮 */
    recorderBt = new QPushButton();
    previousBt = new QPushButton();
    nextBt = new QPushButton();
    removeBt = new QPushButton();

    recorderBt->setCheckable(true);
    recorderBt->setObjectName("recorderBt");
    recorderBt->setFocusPolicy(Qt::NoFocus);
    recorderBt->setMaximumSize(60, 60);
    recorderBt->setMinimumSize(60, 60);

    hBoxLayout->setContentsMargins(0, 0, 0, 0);

    bottomWidget->setLayout(hBoxLayout);
    hBoxLayout->addWidget(recorderBt);
    hBoxLayout->addWidget(previousBt);
    hBoxLayout->addWidget(nextBt);
    hBoxLayout->addWidget(removeBt);

    nextBt->setMaximumSize(50, 50);
    removeBt->setMaximumSize(50, 50);
    previousBt->setMaximumSize(50, 50);

    previousBt->setObjectName("previousBt");
    removeBt->setObjectName("removeBt");
    nextBt->setObjectName("nextBt");

    previousBt->setFocusPolicy(Qt::NoFocus);
    removeBt->setFocusPolicy(Qt::NoFocus);
    nextBt->setFocusPolicy(Qt::NoFocus);

    /* 显示录音时长与录音Level */
    centerWidget = new QWidget(this);
    centerWidget->setGeometry(width()/ 2 - 150,
                              height() /2 - 100,
                              300,
                              200);
    centerWidget->setStyleSheet("QWidget { background:#8823242a;"
                                "border-radius:10px}");
    countLabel = new QLabel(centerWidget);
    countLabel->setGeometry(0,
                            0,
                            300,
                            50);
    countLabel->setStyleSheet("QLabel {font-size: 30px;color:#eeeeee;"
                              "font: blod;background:transparent}");
    countLabel->setAlignment(Qt::AlignCenter);
    levelHBoxLayout = new QHBoxLayout();

    for (int i = 0; i < 4; i++) {
        progressBar[i] = new QProgressBar();
        progressBar[i]->setOrientation(Qt::Vertical);
        progressBar[i]->setRange(0, 100);
        progressBar[i]->setVisible(false);
        progressBar[i]->setMaximumWidth(centralWidget()->width());
        levelHBoxLayout->addWidget(progressBar[i]);
        levelHBoxLayout->setContentsMargins(5, 50, 5, 5);
        progressBar[i]->setStyleSheet("QWidget { background:#22eeeeee;"
                                      "border-radius:0px}");
    }
    centerWidget->setLayout(levelHBoxLayout);
    centerWidget->hide();
    countLabel->raise();


}

void AudioRecorder::recorderBtClicked()
{
    /* 录音前停止正在播放的媒体 */
    if (recorderPlayer->state() != QMediaPlayer::StoppedState)
        recorderPlayer->stop();
    /* 如果录音已经停止，则开始录音 */
    if (m_audioRecorder->state() == QMediaRecorder::StoppedState) {
        /* 设置默认的录音设备 */
        m_audioRecorder->setAudioInput(devicesVar.at(0).toString());

        /* 下面的是录音设置，都是选择默认,可根据录音可用项，自行修改 */
        QAudioEncoderSettings settings;
        settings.setCodec(codecsVar.at(0).toString());
        settings.setSampleRate(sampleRateVar[0].toInt());
        settings.setBitRate(bitratesVar[0].toInt());
        settings.setChannelCount(channelsVar[0].toInt());
        settings.setQuality(QMultimedia::EncodingQuality(
                                qualityVar[0].toInt()));
        /* 以恒定的质量录制，可选恒定的比特率 */
        settings.setEncodingMode(QMultimedia::ConstantQualityEncoding);
        QString container = containersVar.at(0).toString();
        m_audioRecorder->setEncodingSettings(settings,
                                             QVideoEncoderSettings(),
                                             container);
        m_audioRecorder->setOutputLocation(
                    QUrl::fromLocalFile(tr("./Sounds/%1.mp3")
                                        .arg(QDateTime::currentDateTime()
                                             .toString())));
        /* 开始录音 */
        m_audioRecorder->record();
        /* 显示录制时长标签 */
        countLabel->clear();
        centerWidget->show();
    } else {
        /* 停止录音 */
        m_audioRecorder->stop();
        /* 重设录音level */
        clearAudioLevels();
        /* 隐藏录制时长标签 */
        centerWidget->hide();
        /* 重新扫描录音文件 */
        scanRecordFiles();
    }
}

void AudioRecorder::scanRecordFiles()
{
    mediaPlaylist->clear();
    listWidget->clear();
    mediaObjectInfo.clear();
    /* 录音文件保存在当前Sounds文件夹下 */
    QDir dir(QCoreApplication::applicationDirPath()
             + "/Sounds");
    QDir dirbsolutePath(dir.absolutePath());

    /* 如果文件夹不存在，则创建一个 */
    if(!dirbsolutePath.exists())
        dirbsolutePath.mkdir(dirbsolutePath.absolutePath());

    /* 定义过滤器 */
    QStringList filter;
    /* 包含所有xx后缀的文件 */
    filter<<"*.mp3";
    /* 获取该目录下的所有文件 */
    QFileInfoList files =
            dirbsolutePath.entryInfoList(filter, QDir::Files);
    /* 遍历 */
    for (int i = 0; i < files.count(); i++) {
        MediaObjectInfo info;
        /* 使用utf-8编码 */
        info.fileName = QString::fromUtf8(files.at(i)
                                          .fileName()
                                          .toUtf8()
                                          .data());
        info.filePath = QString::fromUtf8(files.at(i)
                                          .filePath()
                                          .toUtf8()
                                          .data());
        /* 媒体列表添加音频 */
        if (mediaPlaylist->addMedia(
                    QUrl::fromLocalFile(info.filePath))) {
            /* 添加到容器数组里储存 */
            mediaObjectInfo.append(info);
            /* 添加音频名字至列表 */
            listWidget->addItem(
                        new QListWidgetItem(QIcon(":/icons/play.png"),
                                            info.fileName));
        } else {
            qDebug()<<
                       mediaPlaylist->errorString()
                       .toUtf8().data()
                    << endl;
            qDebug()<< "  Error number:"
                    << mediaPlaylist->error()
                    << endl;
        }
    }
}

void AudioRecorder::listWidgetCliked(QListWidgetItem *item)
{
    /* item->setIcon 为设置列表里的图标状态 */
    for (int i = 0; i < listWidget->count(); i++) {
        listWidget->item(i)->setIcon(QIcon(":/icons/play.png"));
    }

    if (recorderPlayer->state() != QMediaPlayer::PlayingState) {
        recorderPlayer->play();
        item->setIcon(QIcon(":/icons/pause.png"));
    } else {
        recorderPlayer->pause();
        item->setIcon(QIcon(":/icons/play.png"));
    }
}

void AudioRecorder::listWidgetCurrentItemChange(
        QListWidgetItem *currentItem,
        QListWidgetItem *previousItem)
{
    if (mediaPlaylist->mediaCount() == 0)
        return;

    if (listWidget->row(previousItem) != -1)
        previousItem->setText(mediaObjectInfo
                              .at(listWidget->row(previousItem))
                              .fileName);

    /* 先暂停播放媒体 */
    if (recorderPlayer->state() == QMediaPlayer::PlayingState)
        recorderPlayer->pause();

    /* 设置当前媒体 */
    mediaPlaylist->
            setCurrentIndex(listWidget->row(currentItem));
}

void AudioRecorder::mediaPlayerStateChanged(
        QMediaPlayer::State
        state)
{
    for (int i = 0; i < listWidget->count(); i++) {
        listWidget->item(i)
                ->setIcon(QIcon(":/icons/play.png"));
    }

    /* 获取当前项，根据当前媒体的状态，然后设置不同的图标 */
    if (mediaPlaylist->currentIndex() == -1)
        return;
    QListWidgetItem *item = listWidget->item(
                mediaPlaylist->currentIndex());

    switch (state) {
    case QMediaPlayer::PausedState:
    case QMediaPlayer::PlayingState:
        item->setIcon(QIcon(":/icons/pause.png"));
        break;
    case QMediaPlayer::StoppedState:
        item->setIcon(QIcon(":/icons/play.png"));
        break;
    }
}

void AudioRecorder::mediaPlaylistCurrentIndexChanged(
        int index)
{
    if (-1 == index)
        return;
}

void AudioRecorder::previousBtClicked()
{
    /* 上一首操作 */
    recorderPlayer->stop();
    int count = listWidget->count();
    if (0 == count)
        return;
    if (listWidget->currentRow() == -1)
        listWidget->setCurrentRow(0);
    else {
        if (listWidget->currentRow() - 1 != -1)
            listWidget->setCurrentRow(
                        listWidget->currentRow() - 1);
        else
            listWidget->setCurrentRow(listWidget->count() - 1);
    }
    mediaPlaylist->setCurrentIndex(listWidget->currentRow());
    recorderPlayer->play();
}

void AudioRecorder::nextBtClicked()
{
    /* 下一首操作 */
    recorderPlayer->stop();

    /* 获取列表的总数目 */
    int count = listWidget->count();

    /* 如果列表的总数目为0则返回 */
    if (0 == count)
        return;

    if (listWidget->currentRow() == -1)
        listWidget->setCurrentRow(0);
    else {
        if (listWidget->currentRow() + 1 < listWidget->count())
            listWidget->setCurrentRow(
                        listWidget->currentRow() + 1);
        else
            listWidget->setCurrentRow(0);
    }
    mediaPlaylist->setCurrentIndex(listWidget->currentRow());
    recorderPlayer->play();
}

void AudioRecorder::removeBtClicked()
{
    int index = listWidget->currentRow();
    if (index == -1)
        return;

    /* 移除媒体的项 */
    mediaPlaylist->removeMedia(index);

    /* 指向要删除的文件 */
    QFile file(mediaObjectInfo.at(index).filePath);

    /* 移除录音文件 */
    file.remove();

    /* 删除列表选中的项 */
    listWidget->takeItem(index);

    /* 删除后设置当前项为删除项的前一个 */
    if (index - 1 != -1)
        listWidget->setCurrentRow(index - 1);
}

void AudioRecorder::updateProgress(qint64 duration)
{
    if (m_audioRecorder->error()
            != QMediaRecorder::NoError)
        return;

    /* 显示录制时长 */
    countLabel->setText(tr("已录制 %1 s")
                        .arg(duration / 1000));
}

void AudioRecorder::recorderPlayerPositionChanged(
        qint64 position)
{
    /* 格式化时间 */
    int p_second  = position / 1000;
    int p_minute = p_second / 60;
    p_second %= 60;

    QString mediaPosition;
    mediaPosition.clear();

    if (p_minute >= 10)
        mediaPosition = QString::number(p_minute, 10);
    else
        mediaPosition = "0" + QString::number(p_minute, 10);

    if (p_second >= 10)
        mediaPosition = mediaPosition
                + ":" + QString::number(p_second, 10);
    else
        mediaPosition = mediaPosition
                + ":0" + QString::number(p_second, 10);


    int d_second  =  recorderPlayer->duration() / 1000;
    int d_minute = d_second / 60;
    d_second %= 60;

    QString mediaDuration;
    mediaDuration.clear();

    if (d_minute >= 10)
        mediaDuration = QString::number(d_minute, 10);
    else
        mediaDuration = "0" + QString::number(d_minute, 10);

    if (d_second >= 10)
        mediaDuration = mediaDuration
                + ":" + QString::number(d_second, 10);
    else
        mediaDuration = mediaDuration
                + ":0" + QString::number(d_second, 10);

    QString fileNmae = mediaObjectInfo
            .at(listWidget->currentRow()).fileName + "\t";
    /* 显示媒体总长度时间与播放的当前位置 */
    listWidget->currentItem()->setText(fileNmae
                                       + mediaPosition
                                       +"/" + mediaDuration);
}

void AudioRecorder::clearAudioLevels()
{
    for (int i = 0; i < 4; i++)
        progressBar[i]->setValue(0);
}

// This function returns the maximum possible sample value for a given audio format
qreal getPeakValue(const QAudioFormat& format)
{
    // Note: Only the most common sample formats are supported
    if (!format.isValid())
        return qreal(0);

    if (format.codec() != "audio/pcm")
        return qreal(0);

    switch (format.sampleType()) {
    case QAudioFormat::Unknown:
        break;
    case QAudioFormat::Float:
        if (format.sampleSize() != 32) // other sample formats are not supported
            return qreal(0);
        return qreal(1.00003);
    case QAudioFormat::SignedInt:
        if (format.sampleSize() == 32)
            return qreal(INT_MAX);
        if (format.sampleSize() == 16)
            return qreal(SHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(CHAR_MAX);
        break;
    case QAudioFormat::UnSignedInt:
        if (format.sampleSize() == 32)
            return qreal(UINT_MAX);
        if (format.sampleSize() == 16)
            return qreal(USHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(UCHAR_MAX);
        break;
    }

    return qreal(0);
}

// returns the audio level for each channel
QVector<qreal> getBufferLevels(const QAudioBuffer& buffer)
{
    QVector<qreal> values;

    if (!buffer.format().isValid() || buffer.format().byteOrder() != QAudioFormat::LittleEndian)
        return values;

    if (buffer.format().codec() != "audio/pcm")
        return values;

    int channelCount = buffer.format().channelCount();
    values.fill(0, channelCount);
    qreal peak_value = getPeakValue(buffer.format());
    if (qFuzzyCompare(peak_value, qreal(0)))
        return values;

    switch (buffer.format().sampleType()) {
    case QAudioFormat::Unknown:
    case QAudioFormat::UnSignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<quint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<quint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<quint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] = qAbs(values.at(i) - peak_value / 2) / (peak_value / 2);
        break;
    case QAudioFormat::Float:
        if (buffer.format().sampleSize() == 32) {
            values = getBufferLevels(buffer.constData<float>(), buffer.frameCount(), channelCount);
            for (int i = 0; i < values.size(); ++i)
                values[i] /= peak_value;
        }
        break;
    case QAudioFormat::SignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<qint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<qint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<qint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] /= peak_value;
        break;
    }

    return values;
}

template <class T>
QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels)
{
    QVector<qreal> max_values;
    max_values.fill(0, channels);

    for (int i = 0; i < frames; ++i) {
        for (int j = 0; j < channels; ++j) {
            qreal value = qAbs(qreal(buffer[i * channels + j]));
            if (value > max_values.at(j))
                max_values.replace(j, value);
        }
    }

    return max_values;
}

void AudioRecorder::processBuffer(const QAudioBuffer& buffer)
{
    /* 根据通道数目需要显示count个level */
    int count = buffer.format().channelCount();
    for (int i = 0; i < 4; i++) {
        if (i < count)
            progressBar[i]->setVisible(true);
        else
            progressBar[i]->setVisible(false);
    }

    /* 设置level的值 */
    QVector<qreal> levels = getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i)
        progressBar[i]->setValue(levels.at(i) * 100);
}
