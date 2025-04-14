/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   audiorecorder
* @brief         audiorecorder.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-06-04
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
{
    Q_UNUSED(parent);

    /* 录制音频的类 */
    m_audioRecorder = new QAudioRecorder(this);

    /* 用于探测缓冲区的数据 */
    m_probe = new QAudioProbe(this);

    /* 信号槽连接，更新录音level显示 */
    connect(m_probe, &QAudioProbe::audioBufferProbed,
            this, &AudioRecorder::processBuffer);

    /* 设置探测的对象 */
    m_probe->setSource(m_audioRecorder);

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
    /* 百度语音识别只支持8000、 16000采样率 */
    sampleRateVar.append(QVariant(8000));
    sampleRateVar.append(QVariant(16000));
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

    /* 录音类信号槽连接 */
    connect(m_audioRecorder, &QAudioRecorder::durationChanged,
            this, &AudioRecorder::updateProgress);
}

AudioRecorder::~AudioRecorder()
{
}


void AudioRecorder::startRecorder()
{
    /* 备注：录音需要设置成16000 采样率和通道数为1，
     * 保存为wav文件需要设置成audio/x-wav（container文件格式） */

    /* 如果录音已经停止，则开始录音 */
    if (m_audioRecorder->state() == QMediaRecorder::StoppedState) {
        /* 设置默认的录音设备 */
        m_audioRecorder->setAudioInput(devicesVar.at(0).toString());

        /* 下面的是录音设置 */
        QAudioEncoderSettings settings;
        settings.setCodec(codecsVar.at(0).toString());
        settings.setSampleRate(sampleRateVar[2].toInt());
        settings.setBitRate(bitratesVar[0].toInt());
        settings.setChannelCount(channelsVar[1].toInt());
        settings.setQuality(QMultimedia::EncodingQuality(
                                qualityVar[0].toInt()));

        /* 以恒定的质量录制，可选恒定的比特率 */
        settings.setEncodingMode(QMultimedia::ConstantQualityEncoding);

        /* I.MX6ULL第20个支持的格式为 audio/x-wav */
        QString container = containersVar.at(20).toString();

        /* 使用配置 */
        m_audioRecorder->setEncodingSettings(settings,
                                             QVideoEncoderSettings(),
                                             container);
        /* 录音保存为16k.wav文件 */
        m_audioRecorder->setOutputLocation(QUrl::fromLocalFile(tr("./16k.wav")));

        /* 开始录音 */
        m_audioRecorder->record();
    }
}

void AudioRecorder::stopRecorder()
{
    /* 停止录音 */
    m_audioRecorder->stop();
}


void AudioRecorder::updateProgress(qint64 duration)
{
    Q_UNUSED(duration);

    if (m_audioRecorder->error()
            != QMediaRecorder::NoError)
        return;

    /* 打印录制时长 */
    //qDebug()<<duration / 1000<<endl;
}


void AudioRecorder::clearAudioLevels()
{
    //...
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
    /* 打印通道数 */
    Q_UNUSED(count);
    // qDebug()<<"通道数"<<count<<endl;

    /* 设置level的值 */
    QVector<qreal> levels = getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i) {
        /* 打印音量等级 */
        // qDebug()<<"音量等级"<<levels.at(i)<<endl;
    }
}
