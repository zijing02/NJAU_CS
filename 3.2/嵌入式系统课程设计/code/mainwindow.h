#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort> //串口类
#include <QSerialPortInfo> //串口信息类
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStatusBar>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>
#include "camerawindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;

private:
    QFile LedFile;
    QFile BeepFile;
    void judgeInfo(QByteArray data);
    void setLedState();
    bool getLedState();
    void setBeepState();
    bool getBeepState();
    void openCamera();
    void updateCameraFrame();
    QTimer *rotationTimer;
    int rotationCount = 0;
    int totalRotations = 0;
    QTimer *cameraTimer;
    CameraWindow *cameraWindow = nullptr;
    bool isCameraWindowVisible = false;

private slots:
    void readData();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_checkBox_2_stateChanged(int arg1);
    void on_checkBox_3_stateChanged(int arg1);
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_4_clicked();
    void on_checkBox_stateChanged(int arg1);
    void sendRotationData();
};
#endif // MAINWINDOW_H
