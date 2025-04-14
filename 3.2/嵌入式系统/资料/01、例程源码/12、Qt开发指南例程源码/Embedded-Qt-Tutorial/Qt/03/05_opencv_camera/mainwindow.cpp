/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   05_opencv_camera
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-17
*******************************************************************/
#include "mainwindow.h"
#include <QGuiApplication>
#include <QScreen>
#include <QFile>
#include <QPixmap>
#include <QBuffer>
#include "camera.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 布局初始化 */
    layoutInit();

    /* 扫描摄像头 */
    scanCameraDevice();
}

MainWindow::~MainWindow()
{
}

void MainWindow::layoutInit()
{
    /* 获取屏幕的分辨率，Qt官方建议使用这
     * 种方法获取屏幕分辨率，防上多屏设备导致对应不上
     * 注意，这是获取整个桌面系统的分辨率
     */
    QList <QScreen *> list_screen =  QGuiApplication::screens();

    /* 如果是ARM平台，直接设置大小为屏幕的大小 */
#if __arm__
    /* 重设大小 */
    this->resize(list_screen.at(0)->geometry().width(),
                 list_screen.at(0)->geometry().height());
#else
    /* 否则则设置主窗体大小为800x480 */
    this->resize(800, 480);
#endif

    /* 实例化与布局，常规操作 */
    mainWidget = new QWidget();
    photoLabel = new QLabel();
    rightWidget = new QWidget();
    comboBox = new QComboBox();
    pushButton[0] = new QPushButton();
    pushButton[1] = new QPushButton();
    scrollArea = new QScrollArea();
    displayLabel = new QLabel(scrollArea);
    vboxLayout = new QVBoxLayout();
    hboxLayout = new QHBoxLayout();

    vboxLayout->addWidget(photoLabel);
    vboxLayout->addWidget(comboBox);
    vboxLayout->addWidget(pushButton[0]);
    vboxLayout->addWidget(pushButton[1]);

    rightWidget->setLayout(vboxLayout);

    hboxLayout->addWidget(scrollArea);
    hboxLayout->addWidget(rightWidget);
    mainWidget->setLayout(hboxLayout);

    this->setCentralWidget(mainWidget);

    pushButton[0]->setMaximumHeight(40);
    pushButton[0]->setMaximumWidth(200);

    pushButton[1]->setMaximumHeight(40);
    pushButton[1]->setMaximumWidth(200);

    comboBox->setMaximumHeight(40);
    comboBox->setMaximumWidth(200);
    photoLabel->setMaximumSize(100, 75);
    scrollArea->setMinimumWidth(this->width()
                                - comboBox->width());

    /* 显示图像最大画面为xx */
    displayLabel->setMinimumWidth(scrollArea->width() * 0.75);
    displayLabel->setMinimumHeight(scrollArea->height() * 0.75);
    scrollArea->setWidget(displayLabel);

    /* 居中显示 */
    scrollArea->setAlignment(Qt::AlignCenter);

    /* 自动拉伸 */
    photoLabel->setScaledContents(true);
    displayLabel->setScaledContents(true);

    /* 设置一些属性 */
    pushButton[0]->setText("拍照");
    pushButton[0]->setEnabled(false);
    pushButton[1]->setText("开始");
    pushButton[1]->setCheckable(true);

    /* 摄像头 */
    camera = new Camera(this);

    /* 信号连接槽 */
    connect(camera, SIGNAL(readyImage(QImage)),
            this, SLOT(showImage(QImage)));
    connect(pushButton[1], SIGNAL(clicked(bool)),
            camera, SLOT(cameraProcess(bool)));
    connect(pushButton[1], SIGNAL(clicked(bool)),
            this, SLOT(setButtonText(bool)));
    connect(pushButton[0], SIGNAL(clicked()),
            this, SLOT(saveImageToLocal()));

}

void MainWindow::scanCameraDevice()
{
    /* 如果是Windows系统，一般是摄像头0 */
#if win32
    comboBox->addItem("windows摄像头0");
    connect(comboBox,
            SIGNAL(currentIndexChanged(int)),
            camera, SLOT(selectCameraDevice(int)));
#else
    /* QFile文件指向/dev/video0 */
    QFile file("/dev/video0");

    /* 如果文件存在 */
    if (file.exists())
        comboBox->addItem("video0");
    else {
        displayLabel->setText("无摄像头设备");
        return;
    }

    file.setFileName("/dev/video1");

    if (file.exists()) {
        comboBox->addItem("video1");
        /* 开发板ov5640等设备是1 */
        comboBox->setCurrentIndex(1);
    }

    file.setFileName("/dev/video2");

    if (file.exists())
        /* 开发板USB摄像头设备是2 */
        comboBox->addItem("video2");

#if !__arm__
    /* ubuntu的USB摄像头一般是0 */
    comboBox->setCurrentIndex(0);
#endif

    connect(comboBox,
            SIGNAL(currentIndexChanged(int)),
            camera, SLOT(selectCameraDevice(int)));
#endif
}

void MainWindow::showImage(const QImage &image)
{
    /* 显示图像 */
    displayLabel->setPixmap(QPixmap::fromImage(image));
    saveImage = image;

    /* 判断图像是否为空，空则设置拍照按钮不可用 */
    if (!saveImage.isNull())
        pushButton[0]->setEnabled(true);
    else
        pushButton[0]->setEnabled(false);
}

void MainWindow::setButtonText(bool bl)
{
    if (bl) {
        /* 设置摄像头设备 */
        camera->selectCameraDevice(comboBox->currentIndex());
        pushButton[1]->setText("关闭");
    } else {
        /* 若关闭了摄像头则禁用拍照按钮 */
        pushButton[0]->setEnabled(false);
        pushButton[1]->setText("开始");
    }
}

void MainWindow::saveImageToLocal()
{
    /* 判断图像是否为空 */
    if (!saveImage.isNull()) {
        QString fileName =
                QCoreApplication::applicationDirPath() + "/test.png";
        qDebug()<<"正在保存"<<fileName<<"图片,请稍候..."<<endl;

        /* save(arg1，arg2，arg3)重载函数，arg1代表路径文件名，
         * arg2保存的类型，arg3代表保存的质量等级 */
        saveImage.save(fileName, "PNG", -1);

        /* 设置拍照的图像为显示在photoLabel上 */
        photoLabel->setPixmap(QPixmap::fromImage(QImage(fileName)));

        qDebug()<<"保存完成！"<<endl;
    }
}
