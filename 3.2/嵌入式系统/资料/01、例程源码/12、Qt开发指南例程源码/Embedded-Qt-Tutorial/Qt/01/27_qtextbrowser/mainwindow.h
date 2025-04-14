#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    /* 声明对象 */
    QTextBrowser *textBrowser;
    QAction *openAction;

private slots:
    /* 槽函数 */
    void openActionTriggered();
};
#endif // MAINWINDOW_H
