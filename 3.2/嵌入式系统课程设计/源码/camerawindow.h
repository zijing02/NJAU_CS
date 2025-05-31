#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <QWidget>
#include <QLabel>
#include <opencv2/opencv.hpp>
#include <QTimer>

class CameraWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWindow(QWidget *parent = nullptr);
    void startCamera();
    void stopCamera();

private slots:
    void updateFrame();

private:
    QLabel *imageLabel;
        cv::VideoCapture camera;
        QTimer *timer;
};

#endif // CAMERAWINDOW_H
