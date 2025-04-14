/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   18_sqlite_table
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-18
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMainWindow>
#include <QLabel>
#include <QSqlTableModel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTableView>
#include <QComboBox>
#include <QLineEdit>
#include <QDataWidgetMapper>
#include <QSqlQueryModel>
#include <QItemSelectionModel>
#include <QSpinBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    /* 数据库连接类 */
    QSqlDatabase sqlDatabase;

    /* 用于查询数据 */
    QSqlQueryModel *sqlQueryModel;

    /* 数据映射 */
    QDataWidgetMapper *dataWidgetMapper;

    /* 选择模型 */
    QItemSelectionModel * itemSelectionModel;

    /* 水平布局 */
    QHBoxLayout *hBoxLayout[2];

    /* 垂直布局 */
    QVBoxLayout *vBoxLayout;

    /* 网格布局 */
    QGridLayout *gridLayout;

    /* 用于显示的表格*/
    QTableView *tableView;

    /* 主Widget */
    QWidget *mainWidget;

    /* 底部容器 */
    QWidget *bottomWidget;

    /* 底部网格布局容器 */
    QWidget *gridWidget;

    /* 照片容器 */
    QWidget *photoWidget;

    /* Label，用于显示照片 */
    QLabel *imageLabel;

    /* Label,底部显示文本 */
    QLabel *label[4];

    /* 性别下拉选择框，选择信息 */
    QComboBox *comboBox;

    /* 数值选择框，[0, 100] */
    QSpinBox *spinBox[2];

    /* 单行输入框  */
    QLineEdit *lineEdit;

private slots:
    /* 表格当前行变化执行的槽函数 */
    void on_currentRowChanged(const QModelIndex&, const QModelIndex&);
};
#endif // MAINWINDOW_H
