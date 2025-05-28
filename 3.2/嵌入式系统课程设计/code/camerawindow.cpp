#include "camerawindow.h"
#include <QVBoxLayout>

CameraWindow::CameraWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("摄像头窗口");
    resize(640, 480);

    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(imageLabel);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CameraWindow::updateFrame);
}

void CameraWindow::startCamera()
{
    camera.open(1);
    if(camera.isOpened()) {
        timer->start(33); // ~30fps
    }
}

void CameraWindow::stopCamera()
{
    timer->stop();
    camera.release();
    close();
}

void CameraWindow::updateFrame()
{
    cv::Mat frame;
    if(camera.read(frame)) {
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage img(frame.data, frame.cols, frame.rows,
                 frame.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(img).scaled(
            imageLabel->size(), Qt::KeepAspectRatio));
    }
}
