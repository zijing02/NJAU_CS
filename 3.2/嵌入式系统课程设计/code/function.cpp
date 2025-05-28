#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QTimer>


void MainWindow::setLedState()
{
    bool state = getLedState();

    if (!LedFile.exists())
        return;

    if(!LedFile.open(QIODevice::ReadWrite))
        qDebug()<<LedFile.errorString();

    QByteArray buf[2] = {"0", "1"};

    if (state)
        LedFile.write(buf[0]);
    else
        LedFile.write(buf[1]);

    LedFile.close();
}

bool MainWindow::getLedState()
{
    if (!LedFile.exists())
        return false;

    if(!LedFile.open(QIODevice::ReadWrite))
        qDebug()<<LedFile.errorString();

    QTextStream in(&LedFile);

    QString buf = in.readLine();

    LedFile.close();
    if (buf == "1") {
        return true;
    } else {
        return false;
    }
}

void MainWindow::setBeepState()
{
    bool state = getBeepState();

    if (!BeepFile.exists())
        return;

    if(!BeepFile.open(QIODevice::ReadWrite))
        qDebug()<<BeepFile.errorString();

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

    if(!BeepFile.open(QIODevice::ReadWrite))
        qDebug()<<BeepFile.errorString();

    QTextStream in(&BeepFile);

    QString buf = in.readLine();

    BeepFile.close();
    if (buf == "1") {
        return true;
    } else {
        return false;
    }
}

void MainWindow::judgeInfo(QByteArray data){
    if(data.contains("led") || data.contains("LED") || data.contains("6C 65 64") || data.contains("4C 45 44")){
        LedFile.setFileName("/sys/devices/platform/leds/leds/sys-led/brightness");
        if (!LedFile.exists())
            ui->textBrowser->insertPlainText("未获取到LED设备");
        setLedState();
    }
    else if(data.contains("beep") || data.contains("BEEP") || data.contains("62 65 65 70") || data.contains("42 45 45 50")){
        BeepFile.setFileName("/sys/devices/platform/leds/leds/beep/brightness");
        if (!BeepFile.exists())
            ui->textBrowser->insertPlainText("未获取到BEEP设备！");
        setBeepState();
    }
    else if(data.contains("camera") || data.contains("CAMERA") || data.contains("63 61 6D 65 72 61") || data.contains("43 41 4D 45 52 51")){
        if(!isCameraWindowVisible) {
                   // 创建并显示窗口
                   if(!cameraWindow) {
                       cameraWindow = new CameraWindow(this);
                   }
                   cameraWindow->startCamera();
                   cameraWindow->show();
                   isCameraWindowVisible = true;
               } else {
                   // 关闭窗口
                   cameraWindow->stopCamera();
                   isCameraWindowVisible = false;
               }
           }
}

