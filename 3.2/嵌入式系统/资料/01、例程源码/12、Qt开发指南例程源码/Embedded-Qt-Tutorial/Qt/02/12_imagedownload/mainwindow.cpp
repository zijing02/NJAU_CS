/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   12_imagedownload
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-04-16
*******************************************************************/
#include "mainwindow.h"
#include <QMessageBox>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 标签0, 显示下载的图像 */
    label[0] = new QLabel();
    /* 标签1, 显示URL标签 */
    label[1] = new QLabel();
    /* 下载进度标签 */
    label[2] = new QLabel();

    /* 下载图片链接输入框 */
    lineEdit = new QLineEdit();

    /* 下载按钮 */
    pushButton = new QPushButton();

    /* 下载进度条 */
    progressBar = new QProgressBar();

    /* 水平布局 */
    hBoxLayout[0] = new QHBoxLayout();
    hBoxLayout[1] = new QHBoxLayout();

    /* 垂直布局 */
    vBoxLayout = new QVBoxLayout();

    /* 水平容器 */
    hWidget[0] = new QWidget();
    hWidget[1] = new QWidget();

    /* 垂直容器 */
    vWidget = new QWidget();

    label[1]->setText("URL链接：");
    label[2]->setText("文件下载进度：");

    pushButton->setText("下载");

    /* 设置下载链接地址,在浏览器右键复制图片下载地址即可 */
    lineEdit->setText("https://img1.baidu.com/it/u=3908406816,2850982836&"
                      "fm=253&fmt=auto&app=120&f=JPEG?w=889&h=500");
    /* 设置标签的最小显示大小 */
    label[0]->setMinimumSize(this->width(),
                             this->height() * 0.75);

    /* 根据文本文字大小自适应大小 */
    label[1]->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);
    label[2]->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);
    pushButton->setSizePolicy(QSizePolicy::Fixed,
                              QSizePolicy::Fixed);

    /* 水平布局0添加元素 */
    hBoxLayout[0]->addWidget(label[1]);
    hBoxLayout[0]->addWidget(lineEdit);
    hBoxLayout[0]->addWidget(pushButton);

    /* 设置水平布局0为水平容器的布局0 */
    hWidget[0]->setLayout(hBoxLayout[0]);

    /* 水平布局1添加元素 */
    hBoxLayout[1]->addWidget(label[2]);
    hBoxLayout[1]->addWidget(progressBar);

    /* 设置水平布局1为水平容器的布局1 */
    hWidget[1]->setLayout(hBoxLayout[1]);

    /* 垂直布局添加元素 */
    vBoxLayout->addWidget(label[0]);
    vBoxLayout->addWidget(hWidget[0]);
    vBoxLayout->addWidget(hWidget[1]);

    /* 设置垂直布局为垂直容器的布局 */
    vWidget->setLayout(vBoxLayout);

    /* 设置居中 */
    setCentralWidget(vWidget);

    /* 网络管理 */
    networkAccessManager = new QNetworkAccessManager(this);

    /* 信号槽连接 */
    connect(pushButton, SIGNAL(clicked()),
            this, SLOT(startDownload()));

}

MainWindow::~MainWindow()
{
}

void MainWindow::startDownload()
{
    /* 获取URL链接 */
    QUrl newUrl(QUrl(lineEdit->text()));

    /* 如果下载链接无效，则直接返回 */
    if (!newUrl.isValid()) {
        QMessageBox::information(this, "error", "invalid url");
        return;
    }

    /* 网络请求 */
    QNetworkRequest  networkRequest;

    /* 设置下载的地址 */
    networkRequest.setUrl(newUrl);

    /* 网络响应 */
    QNetworkReply *newReply =
            networkAccessManager->get(networkRequest);

    /* 信号槽连接 */
    connect(newReply, SIGNAL(finished()),
            this, SLOT(replyFinished()));
    /* 请求成功后，立刻去读，才能成功下载图片 */
    // connect(newReply, SIGNAL(readyRead()),
    //      this, SLOT(readyReadData()));
    connect(newReply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(imageDownloadProgress(qint64, qint64)));
    connect(newReply,
            SIGNAL(error(QNetworkReply::NetworkError)),
            this,
            SLOT(networkReplyError(QNetworkReply::NetworkError )));
}

void MainWindow::readyReadData()
{
    /* 设置按钮不可用，防止未完成，再次点击 */
    pushButton->setEnabled(false);

    /* 获取信号发送者 */
    QNetworkReply *reply = (QNetworkReply *)sender();

    QFile imageFile;
    /* 保存到当前路径，名称为"下载的.jpg" */
    imageFile.setFileName(QCoreApplication::applicationDirPath()
                          + "/下载的.jpg");

    /* 如果此图片已经存在，则删除 */
    if (imageFile.exists())
        imageFile.remove();

    /* 读取数据 */
    QByteArray data =  reply->readAll();
    /* 如果数据为空，返回 */
    if (data.isEmpty()) {
        qDebug()<<"data is null, please try it again!"<<endl;
        return;
    }

    /* 判断是不是JPG格式的图片，如果不是则返回，如果不需要判断JPG可以删除这部分代码 */
    if (! (data[0] == (char)0xff
           && data[1] == (char)0xd8
           && data[data.size() - 2] == (char)0xff
           && data[data.size() - 1] == (char)0xd9)) {
        qDebug()<<"not JPG data, please try it again!"<<endl;
        return;
    }

    /* 转为QPixmap */
    QPixmap pixmap;
    pixmap.loadFromData(data);
    pixmap.save(imageFile.fileName());
}

void MainWindow::readyReadData(QNetworkReply *reply)
{
    /* 设置按钮不可用，防止未完成，再次点击 */
    pushButton->setEnabled(false);

    QFile imageFile;
    /* 保存到当前路径，名称为"下载的.jpg" */
    imageFile.setFileName(QCoreApplication::applicationDirPath()
                          + "/下载的.jpg");

    /* 如果此图片已经存在，则删除 */
    if (imageFile.exists())
        imageFile.remove();

    /* 读取数据 */
    QByteArray data =  reply->readAll();
    /* 如果数据为空，返回 */
    if (data.isEmpty()) {
        qDebug()<<"data is null, please try it again!"<<endl;
        return;
    }

    /* 判断是不是JPG格式的图片，如果不是则返回 */
    if (! (data[0] == (char)0xff
           && data[1] == (char)0xd8
           && data[data.size() - 2] == (char)0xff
           && data[data.size() - 1] == (char)0xd9)) {
        qDebug()<<"not JPG data, please try it again!"<<endl;
        return;
    }

    /* 转为QPixmap */
    QPixmap pixmap;
    pixmap.loadFromData(data);
    pixmap.save(imageFile.fileName());
}

void MainWindow::replyFinished()
{
    /* 获取信号发送者 */
    QNetworkReply *reply = (QNetworkReply *)sender();

    /* 防止内存泄漏 */
    reply->deleteLater();

    /* 立刻读取数据 */
    readyReadData(reply);

    /* 判断当前执行程序下的图像是否下载完成 */
    QFile imageFile(QCoreApplication::applicationDirPath()
                    + "/下载的.jpg");
    if (imageFile.exists()) {
        /* 显示下载的图像 */
        label[0]->setPixmap(QPixmap(imageFile.fileName()));
        qDebug() <<"已经成功下载，文件路径为:"
                <<imageFile.fileName()<<endl;
    } else
        /* 清空显示 */
        label[0]->clear();

    /* 设置按钮可用 */
    pushButton->setEnabled(true);
}

void MainWindow::imageDownloadProgress(qint64 bytes,
                                       qint64 totalBytes)
{
    /* 设置进度条的最大值 */
    progressBar->setMaximum(totalBytes);
    /* 设置当前值 */
    progressBar->setValue(bytes);
}

/* 网络响应处理函数 */
void MainWindow::networkReplyError(QNetworkReply::NetworkError
                                   error)
{
    switch (error) {
    case QNetworkReply::ConnectionRefusedError:
        qDebug()<<"远程服务器拒绝连接"<<endl;
        break;
    case QNetworkReply::HostNotFoundError:
        qDebug()<<"找不到远程主机名"<<endl;
        break;
    case QNetworkReply::TimeoutError:
        qDebug()<<"与远程服务器连接超时"<<endl;
        break;
    default:
        break;
    }
}
