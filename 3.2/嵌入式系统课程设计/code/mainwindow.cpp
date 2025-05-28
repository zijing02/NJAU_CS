#include "mainwindow.h"
#include "camerawindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serialPort = new QSerialPort(this);

    // 扫描本机下面的串口，并且添加到下拉框里
    foreach (const QSerialPortInfo & info, QSerialPortInfo::availablePorts()) {
        ui->comboBox->addItem(info.portName());
    }

    // 读取数据
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readData()));

    // 初始化定时器
    rotationTimer = new QTimer(this);
    connect(rotationTimer, &QTimer::timeout, this, &MainWindow::sendRotationData);
}

MainWindow::~MainWindow()
{
    if(cameraWindow) {
            cameraWindow->stopCamera();
            delete cameraWindow;
        }
    delete ui;
}


void MainWindow::on_pushButton_clicked(bool checked)
{
    if(checked){
        // 设置串口名字
        serialPort->setPortName(ui->comboBox->currentText());
        // 设置波特率
        serialPort->setBaudRate(ui->comboBox_2->currentText().toInt());
        // 设置校验位
        switch(ui->comboBox_3->currentText().toInt()){
            case 0:
                serialPort->setParity(QSerialPort::NoParity);
                break;
            case 1:
                serialPort->setParity(QSerialPort::EvenParity);
                break;
            case 2:
                serialPort->setParity(QSerialPort::OddParity);
                break;
            case 3:
                serialPort->setParity(QSerialPort::SpaceParity);
                break;
            case 4:
                serialPort->setParity(QSerialPort::MarkParity);
                break;
            default:break;
        }
        // 设置数据位
        serialPort->setDataBits(QSerialPort::DataBits(ui->comboBox_4->currentText().toInt()));
        // 设置停止位
        serialPort->setStopBits(QSerialPort::StopBits(ui->comboBox_5->currentText().toInt()));
        // 设置流控为无
        serialPort->setFlowControl(QSerialPort::NoFlowControl);

        if(!serialPort->open(QIODevice::ReadWrite)){
            QMessageBox::about(this, "错误", "串口打开失败，可能被占用了");
            return;
        }

        ui->comboBox->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
        ui->comboBox_4->setEnabled(false);
        ui->comboBox_5->setEnabled(false);

        ui->pushButton->setText("关闭串口");
    }
    else{
        // 关闭串口
        serialPort->close();

        ui->comboBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox_4->setEnabled(true);
        ui->comboBox_5->setEnabled(true);

        ui->pushButton->setText("打开串口");
    }
}

void MainWindow::readData()
{
    // 接收数据
    QByteArray data = serialPort->readAll();
    ui->textBrowser->insertPlainText(data);
    judgeInfo(data);
}


void MainWindow::on_pushButton_2_clicked()
{
    // 发数据
    serialPort->write(ui->textEdit->toPlainText().toUtf8() + "\n");
}

void MainWindow::on_pushButton_3_clicked()
{
    // 清除接收区数据
    ui->textEdit->clear();
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    QTextEdit* textEdit = ui->textEdit; // textEdit
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender()); // 获取发送信号的checkBox

    if (arg1 == Qt::Checked) {
        // 转换为16进制
        QString originalText = textEdit->toPlainText();
        QString hexText;
        QByteArray utf8Data = originalText.toUtf8();

        for (int i = 0; i < utf8Data.size(); ++i) {
            hexText += QString("%1 ").arg((quint8)utf8Data[i], 2, 16, QChar('0'));
        }
        hexText = hexText.trimmed().toUpper();

        // 阻止信号循环
        textEdit->blockSignals(true);
        textEdit->setPlainText(hexText);
        textEdit->blockSignals(false);
    } else {
        // 转换回UTF-8
        QString hexText = textEdit->toPlainText();
        QStringList hexValues = hexText.split(' ', QString::SkipEmptyParts);
        QByteArray utf8Data;

        bool ok;
        for (const QString &hexValue : hexValues) {
            utf8Data.append((char)hexValue.toInt(&ok, 16));
            if (!ok) {
                // 处理转换错误
                QMessageBox::warning(this, tr("Error"), tr("Invalid hex format!"));
                checkBox->setChecked(false);
                return;
            }
        }

        QString originalText = QString::fromUtf8(utf8Data);

        // 阻止信号循环
        textEdit->blockSignals(true);
        textEdit->setPlainText(originalText);
        textEdit->blockSignals(false);
    }
}

void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    QTextEdit* textEdit = ui->textBrowser; // textBrowser
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender()); // 获取发送信号的checkBox

    if (arg1 == Qt::Checked) {
        // 转换为16进制
        QString originalText = textEdit->toPlainText();
        QString hexText;
        QByteArray utf8Data = originalText.toUtf8();

        for (int i = 0; i < utf8Data.size(); ++i) {
            hexText += QString("%1 ").arg((quint8)utf8Data[i], 2, 16, QChar('0'));
        }
        hexText = hexText.trimmed().toUpper();

        // 阻止信号循环
        textEdit->blockSignals(true);
        textEdit->setPlainText(hexText);
        textEdit->blockSignals(false);
    } else {
        // 转换回UTF-8
        QString hexText = textEdit->toPlainText();
        QStringList hexValues = hexText.split(' ', QString::SkipEmptyParts);
        QByteArray utf8Data;

        bool ok;
        for (const QString &hexValue : hexValues) {
            utf8Data.append((char)hexValue.toInt(&ok, 16));
            if (!ok) {
                // 处理转换错误
                QMessageBox::warning(this, tr("Error"), tr("Invalid hex format!"));
                checkBox->setChecked(false);
                return;
            }
        }

        QString originalText = QString::fromUtf8(utf8Data);

        // 阻止信号循环
        textEdit->blockSignals(true);
        textEdit->setPlainText(originalText);
        textEdit->blockSignals(false);
    }
}


void MainWindow::on_pushButton_6_clicked()
{
    // 获取当前lineEdit中的路径作为初始路径（如果有）
    QString initialPath = ui->lineEdit->text();

    // 如果lineEdit为空或路径不存在，使用用户主目录作为初始路径
    QDir dir(initialPath);
    if(initialPath.isEmpty() || !dir.exists()) {
        initialPath = QDir::homePath();
    }

    // 打开文件夹选择对话框
    QString selectedDir = QFileDialog::getExistingDirectory(
        this,                           // 父窗口
        tr("选择文件夹"),               // 对话框标题
        initialPath,                    // 初始目录
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks  // 选项
    );

    // 如果用户选择了文件夹（没有点击取消）
    if(!selectedDir.isEmpty()) {
        // 将选定的文件夹路径显示在lineEdit中
        ui->lineEdit->setText(QDir::toNativeSeparators(selectedDir));

        // 可选：保存最近使用的文件夹路径
        QSettings settings;
        settings.setValue("LastUsedDir", selectedDir);

        // 可选：输出调试信息
        qDebug() << "Selected folder:" << selectedDir;
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    // 1. 获取路径和文件名
    QString dirPath = ui->lineEdit->text().trimmed();
    QString fileName = ui->lineEdit_3->text().trimmed();

    // 2. 验证路径和文件名是否为空
    if(dirPath.isEmpty() || fileName.isEmpty()) {
        QMessageBox::warning(this, tr("错误"), tr("路径和文件名不能为空！"));
        return;
    }

    // 3. 确保路径存在，不存在则创建
    QDir dir(dirPath);
    if(!dir.exists()) {
        if(!dir.mkpath(".")) {
            QMessageBox::warning(this, tr("错误"), tr("无法创建指定目录！"));
            return;
        }
    }

    // 4. 组合完整文件路径（自动添加.txt扩展名如果不存在）
    if(!fileName.endsWith(".txt", Qt::CaseInsensitive)) {
        fileName += ".txt";
    }
    QString filePath = dir.filePath(fileName);

    // 5. 获取textBrowser内容
    QString content = ui->textBrowser->toPlainText();

    // 6. 写入文件
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << content;
        file.close();

        // 7. 显示成功消息
        QMessageBox::information(this, tr("成功"),
                               tr("文件已保存到:\n%1").arg(QDir::toNativeSeparators(filePath)));
    } else {
        QMessageBox::warning(this, tr("错误"), tr("无法打开文件进行写入！"));
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    // 1. 获取初始路径
    QString initialPath = QDir::homePath();

    // 2. 创建并设置文件对话框
    QFileDialog dialog(this, tr("选择文本文件"));
    dialog.setDirectory(initialPath);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("文本文件 (*.txt);;所有文件 (*)"));
    dialog.setViewMode(QFileDialog::Detail);

    // 3. 执行文件选择
    if(dialog.exec() == QDialog::Accepted) {
        QStringList selectedFiles = dialog.selectedFiles();
        if(!selectedFiles.isEmpty()) {
            QString filePath = selectedFiles.first();

            // 4. 读取并显示文件内容
            QFile file(filePath);
            if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                in.setCodec("UTF-8"); // 设置编码为UTF-8
                QString content = in.readAll();
                file.close();

                // 5. 显示到textEdit
                ui->textEdit->setPlainText(content);

                // 6. 可选：显示状态信息
                statusBar()->showMessage(tr("已加载文件: %1").arg(filePath), 3000);
            } else {
                QMessageBox::warning(this, tr("错误"),
                                   tr("无法打开文件:\n%1").arg(filePath));
            }
        }
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    // 清空接收区
    ui->textBrowser->clear();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    // 是否轮发
    if (arg1 == Qt::Checked) {
        // 获取轮发次数和时间间隔
        totalRotations = ui->spinBox_2->value();
        int interval = ui->spinBox->value();

        // 设置默认值
        if (interval <= 0) {
            interval = 1000; // 默认1000ms
            ui->spinBox->setValue(interval);
        }

        // 初始化计数器
        rotationCount = 0;

        // 启动定时器
        rotationTimer->start(interval);

        // 将spinBox、spinBox2设置为不可修改
        ui->spinBox->setEnabled(false);
        ui->spinBox_2->setEnabled(false);
        ui->checkBox->setEnabled(false);

        // 立即发送第一次
        sendRotationData();
    } else {
        // 停止轮发
        rotationTimer->stop();
        rotationCount = 0;
    }
}

void MainWindow::sendRotationData()
{
    // 轮发数据函数
    if (rotationCount < totalRotations) {
        // 调用发送函数
        on_pushButton_2_clicked();
        rotationCount++;

    } else {
        // 达到指定次数后停止
        rotationTimer->stop();
        ui->checkBox->setChecked(false);

        // 将spinBox、spinBox2设置为可修改
        ui->spinBox->setEnabled(true);
        ui->spinBox_2->setEnabled(true);
        ui->checkBox->setEnabled(true);
    }
}
