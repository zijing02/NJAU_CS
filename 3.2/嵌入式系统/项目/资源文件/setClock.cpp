#include "mainwindow.h"

void MainWindow::setClock()
{
    QTime currentTime = QTime::currentTime();
    QFont bigFont;
    bigFont.setPointSize(24);

    // 创建 QTimeEdit 并设置当前时间
    QTimeEdit *timeEdit = new QTimeEdit(currentTime, this);
    timeEdit->setDisplayFormat("HH:mm:ss");
    timeEdit->setTime(currentTime);
    timeEdit->setMinimumTime(QTime(0, 0, 0));
    timeEdit->setMaximumTime(QTime(23, 59, 59));
    timeEdit->setFont(bigFont);
    timeEdit->setMinimumHeight(60);

    // 创建对话框
    QDialog dialog(this);
    dialog.setWindowTitle("设置闹钟");
    dialog.setFixedSize(600, 400);
    QVBoxLayout layout;
    QLabel label("请选择闹钟时间 (可用上下按钮调整):");
    label.setFont(bigFont);
    label.setMinimumHeight(60);
    layout.addWidget(&label);
    layout.addWidget(timeEdit);

    // 选择铃声方式
    QHBoxLayout *soundLayout = new QHBoxLayout;
    QPushButton *musicBtn = new QPushButton("选择本地音乐");
    QPushButton *beepBtn = new QPushButton("使用蜂鸣器");
    musicBtn->setFont(bigFont);
    beepBtn->setFont(bigFont);
    soundLayout->addWidget(musicBtn);
    soundLayout->addWidget(beepBtn);
    layout.addLayout(soundLayout);

    // 用于记录选择
    QString selectedMusicPath = alarmMusicPath;
    bool useBeep = alarmUseBeep;

    connect(musicBtn, &QPushButton::clicked, [&]()
            {
           QString filePath = QFileDialog::getOpenFileName(&dialog, "选择音乐文件", "", "音频文件 (*.mp3 *.wav *.ogg)");
           if (!filePath.isEmpty()) {
               selectedMusicPath = filePath;
               useBeep = false;
               QFileInfo info(filePath);
               musicBtn->setText(info.fileName()); // 显示文件名
               beepBtn->setText("使用蜂鸣器");
           } });
    connect(beepBtn, &QPushButton::clicked, [&]()
            {
           useBeep = true;
           beepBtn->setText("已选择蜂鸣器");
           musicBtn->setText("选择本地音乐");
           selectedMusicPath.clear(); });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox.setFont(bigFont);
    buttonBox.setMinimumHeight(60);
    layout.addWidget(&buttonBox);
    dialog.setLayout(&layout);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        QTime newTime = timeEdit->time();
        alarmTime = newTime;
        alarmActive = true;
        alarmTimer->start(1000);
        // 保存选择
        alarmUseBeep = useBeep;
        alarmMusicPath = selectedMusicPath;
        qDebug() << "闹钟设置为:" << alarmTime.toString("HH:mm:ss");
        if (!useBeep)
            qDebug() << "闹钟铃声文件:" << alarmMusicPath;
    }
}
void MainWindow::checkAlarm()
{
    if (!alarmActive)
        return;

    QTime currentTime = QTime::currentTime();

    // 检查是否到达闹钟时间
    if (currentTime.hour() == alarmTime.hour() &&
        currentTime.minute() == alarmTime.minute() &&
        currentTime.second() == alarmTime.second())
    {

        qDebug() << "闹钟时间到!";
        alarmActive = false;
        alarmTimer->stop();

        // 根据选择播放
        if (alarmUseBeep)
        {
            BeepFile.setFileName("/sys/devices/platform/leds/leds/beep/brightness");
            if (!BeepFile.exists())
                QMessageBox::warning(this, "错误", "未获取到BEEP设备");
            setBeepState();
        }
        else if (!alarmMusicPath.isEmpty())
        {
            if (alarmPlayer)
            {
                alarmPlayer->setMedia(QUrl::fromLocalFile(alarmMusicPath));
                alarmPlayer->setVolume(100);
                alarmPlayer->play();
            }
        }

        // 显示闹钟提示
        QMessageBox msgBox;
        msgBox.setWindowTitle("闹钟");
        msgBox.setText(QString("现在是 %1，闹钟时间到!").arg(currentTime.toString("HH:mm:ss")));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setButtonText(QMessageBox::Yes, "关闭闹钟");
        msgBox.setButtonText(QMessageBox::No, "5分钟后提醒");
        msgBox.setDefaultButton(QMessageBox::Yes);

        int result = msgBox.exec();
        handleAlarmResponse(result);

        // 停止蜂鸣器或音乐
        if (alarmUseBeep)
        {
            setBeepState();
        }
        else if (!alarmMusicPath.isEmpty())
        {
            if (alarmPlayer)
            {
                alarmPlayer->stop();
            }
        }
    }
}
void MainWindow::handleAlarmResponse(int result)
{
    if (result == QMessageBox::No)
    {
        // 5分钟后重新提醒
        alarmTime = alarmTime.addSecs(5 * 60); // 将闹钟时间增加5分钟
        alarmActive = true;
        alarmTimer->start(1000);
        qDebug() << "闹钟将在5分钟后再次提醒";
    }
    else
    {
        qDebug() << "闹钟已关闭";
    }
}

void MainWindow::setBeepState()
{
    bool state = getBeepState();

    if (!BeepFile.exists())
        return;

    if (!BeepFile.open(QIODevice::ReadWrite))
        qDebug() << BeepFile.errorString();

    QByteArray buf[2] = {"0", "1"};

    if (state)
        BeepFile.write(buf[0]);
    else
        BeepFile.write(buf[1]);

    BeepFile.close();

    getBeepState();
}

bool MainWindow::getBeepState()
{
    if (!BeepFile.exists())
        return false;

    if (!BeepFile.open(QIODevice::ReadWrite))
        qDebug() << BeepFile.errorString();

    QTextStream in(&BeepFile);

    QString buf = in.readLine();

    BeepFile.close();
    if (buf == "1")
    {
        return true;
    }
    else
    {
        return false;
    }
}
