/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   18_sqlite_table
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-18
*******************************************************************/
#include "mainwindow.h"
#include <QDebug>
#include <QHeaderView>
#include <QSqlError>
#include <QApplication>
#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的显示位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 查看本机可用的数据库驱动 */
    QStringList drivers = QSqlDatabase::drivers();
    foreach(QString driver, drivers) {
        qDebug()<<driver;
    }

    /* 以QSQLITE驱动方式打开或者创建数据库 */
    sqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    sqlDatabase.setDatabaseName("employee.db");
    /* 以open的方式打开employee.db数据库，则会创建一个employee.db */
    if (!sqlDatabase.open())
        qDebug()<<"连接数据库错误"<<sqlDatabase.lastError()<<endl;
    else
        qDebug()<<"连接数据库成功"<<endl;

    QSqlQuery query(sqlDatabase);
    /* 使用指令式创建表 */
    query.exec("create table employee (id int primary key, name vchar(10), "
               "age int, sex vchar(3), photo text)");

    QStringList photoPath;
    /* 当前可执行程序的路径 */
    QString path(QApplication::applicationDirPath());
    photoPath<< path + "/photos/啊万.jpg"<< path + "/photos/啊棠.jpg";

    /* 以指令的方式插入数据，如果数据已经存在则不会成功不能插入 */
    query.exec(tr("insert into employee values(1, '啊万', 27, '男', '%1')").arg(photoPath[0]));
    query.exec(tr("insert into employee values(2, '啊棠', 28, '男', '%1')").arg(photoPath[1]));

    //    /* 初始化表格模型 */
    //    QSqlTableModel *model = new QSqlTableModel(this, sqlDatabase);

    //    /* 设置要选中的表格名称 */
    //    model->setTable("employee");
    //    /* 如果有修改则同步修改到数据库，
    //     * 注意这个规则需要与tabview这样的控件才生效，
    //     * 因为tabview可以直接编辑表里的内容 */
    //    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    //    /* 成功则返回true，查看数据库里是否有employee这个表格 */
    //    model->select();
    //    /* 设置表格的头信息，若不设置则显示数据库里的英文字段头信息 */
    //    model->setHeaderData(model->fieldIndex("id"),
    //                         Qt::Horizontal, tr("编号"));
    //    model->setHeaderData(model->fieldIndex("name"),
    //                         Qt::Horizontal, tr("姓名"));
    //    model->setHeaderData(model->fieldIndex("age"),
    //                         Qt::Horizontal, tr("年龄"));
    //    model->setHeaderData(model->fieldIndex("sex"),
    //                         Qt::Horizontal, tr("性别"));

    //    QTableView *view = new QTableView;

    //    /* 设置表格的模型为model */
    //    view->setModel(model);
    //    /* 不显示图片路径信息行 */
    //    view->hideColumn(4);
    //    /* 表格居中 */
    //    setCentralWidget(view);
    //    return;

    /* QSqlQueryModel适合用于查询数据，不能修改数据 */
    sqlQueryModel = new QSqlQueryModel(this);

    /* 选择编号，姓名，年龄和性别的内容，显示到tableView上，
     * 图片最后通过数据选择再读取Label上 */
    sqlQueryModel->setQuery("select id, name, age, sex from employee");

    if (sqlQueryModel->lastError().isValid())
        qDebug()<<"选择数据失败！"<<endl;

    sqlQueryModel->setHeaderData(0, Qt::Horizontal, "编号");
    sqlQueryModel->setHeaderData(1, Qt::Horizontal, "姓名");
    sqlQueryModel->setHeaderData(2, Qt::Horizontal, "年龄");
    sqlQueryModel->setHeaderData(3, Qt::Horizontal, "性别");

    tableView = new QTableView();
    tableView->setModel(sqlQueryModel);

    /* 设置显示平均分列 */
    tableView->horizontalHeader()
            ->setSectionResizeMode(QHeaderView::Stretch);

    mainWidget = new QWidget();
    bottomWidget = new QWidget();
    gridWidget = new QWidget();
    photoWidget = new QWidget();
    imageLabel = new QLabel();

    /* 设置照片属性 */
    imageLabel->setScaledContents(true);
    imageLabel->setMaximumSize(200, 200);


    vBoxLayout = new QVBoxLayout();
    hBoxLayout[0] = new QHBoxLayout();
    hBoxLayout[1] = new QHBoxLayout();
    gridLayout = new QGridLayout();

    for (int i = 0; i < 4; i++)
        label[i] = new QLabel();

    for (int i = 0; i < 2; i++) {
        spinBox[i] = new QSpinBox();
        spinBox[i]->setRange(1, 100);
    }

    comboBox = new QComboBox();
    comboBox->addItem("男");
    comboBox->addItem("女");

    lineEdit = new QLineEdit();

    bottomWidget->setMinimumHeight(this->height() / 2 - 30);
    gridWidget->setMaximumWidth(this->width() / 2 - 30);

    /* 垂直布局 */
    vBoxLayout->addWidget(tableView);
    vBoxLayout->addWidget(bottomWidget);

    mainWidget->setLayout(vBoxLayout);
    setCentralWidget(mainWidget);

    /* 水平布局 */
    hBoxLayout[0]->addWidget(gridWidget);
    hBoxLayout[0]->addWidget(photoWidget);
    bottomWidget->setLayout(hBoxLayout[0]);

    QStringList list;
    list<<"姓名："<<"编号："<<"年龄："<<"性别：";

    /* 网格布局 */
    for (int i = 0; i < 4; i++) {
        gridLayout->addWidget(label[i], i, 0);
        label[i]->setText(list[i]);
        switch (i) {
        case 0:
            gridLayout->addWidget(lineEdit, i, 1);
            break;
        case 1:
            gridLayout->addWidget(spinBox[0], i, 1);
            break;
        case 2:
            gridLayout->addWidget(spinBox[1], i, 1);
            break;
        case 3:
            gridLayout->addWidget(comboBox, i, 1);
            break;
        default:
            break;
        }
    }

    gridWidget->setLayout(gridLayout);
    hBoxLayout[1]->addWidget(imageLabel);
    photoWidget->setLayout(hBoxLayout[1]);

    itemSelectionModel = new QItemSelectionModel(sqlQueryModel);
    tableView->setSelectionModel(itemSelectionModel);

    /* 信号槽连接，表示表中行数据变化时，触发槽函数 */
    connect(itemSelectionModel,
            SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            this,
            SLOT(on_currentRowChanged(QModelIndex, QModelIndex)));

    dataWidgetMapper = new QDataWidgetMapper(this);
    /* 设置为自动提交 */
    dataWidgetMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    dataWidgetMapper->setModel(sqlQueryModel);
    /* 创建数据映射，将前面的数据库内容映射到控件上 */
    dataWidgetMapper->addMapping(lineEdit, 1);
    dataWidgetMapper->addMapping(spinBox[0], 0);
    dataWidgetMapper->addMapping(spinBox[1], 2);
    dataWidgetMapper->addMapping(comboBox, 3);
}

MainWindow::~MainWindow()
{
    /* 关闭数据库 */
    sqlDatabase.close();
}

void MainWindow::on_currentRowChanged(const QModelIndex &current,
                                      const QModelIndex &previous)
{
    Q_UNUSED(previous)
    /* 更新数据映射行号，初始化时映射到第0行 */
    dataWidgetMapper->setCurrentModelIndex(current);
    /* 获取当前行号 */
    int row =  itemSelectionModel->currentIndex().row();
    /* 获取当前模型记录 */
    QSqlRecord record = sqlQueryModel->record(row);
    /* 获取id信息 */
    int id = record.value("id").toInt();
    QSqlQuery query;
    /* 使用bindValue绑定prepare里语句的值，需要使用":"，":"是占位符 */
    query.prepare("select photo from employee where id = :ID");
    query.bindValue(":ID", id);
    query.exec();
    /* 返回到选择的第一条记录，因为id是唯一的，也只有一条记录 */
    query.first();

    /* 获取字段为photo的值，也就是存储照片的路径 */
    QVariant temp = query.value("photo");
    if (!temp.isValid()) {
        qDebug()<<"数据无效！"<<endl;
        return;
    }

    /* 清空图片显示 */
    imageLabel->clear();

    QImage image(temp.toString());

    if (image.isNull()) {
        qDebug()<<"未找到"<<temp.toString()<<endl;
        return;
    }

    /* 显示照片 */
    imageLabel->setPixmap(QPixmap::fromImage(image));
}
