#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

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
    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;
    QAction *openAction;

private slots:
    /* 槽函数 */
    void openActionTriggered();

};
#endif // MAINWINDOW_H
