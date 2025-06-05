#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
        ui->setupUi(this);

        musicPlayer = new QMediaPlayer(this);
        playlist = new QMediaPlaylist(musicPlayer);
        musicPlayer->setPlaylist(playlist);
        alarmPlayer = new QMediaPlayer(this);

        // 页面切换
        connect(ui->pushButton, &QPushButton::clicked, [this]()
                { ui->stackedWidget->setCurrentIndex(0); });
        connect(ui->pushButton_1, &QPushButton::clicked, [this]()
                { ui->stackedWidget->setCurrentIndex(1); initChoiceMode(); });
        connect(ui->pushButton_2, &QPushButton::clicked, [this]()
                { ui->stackedWidget->setCurrentIndex(2); initBlankMode(); });
        connect(ui->pushButton_3, &QPushButton::clicked, [this]()
                { ui->stackedWidget->setCurrentIndex(3); loadMusic(); });
        connect(ui->pushButton_4, &QPushButton::clicked, [this]()
                { ui->stackedWidget->setCurrentIndex(4); loadWrongWords(); });
        connect(ui->pushButton_5, &QPushButton::clicked, [this]()
                { ui->stackedWidget->setCurrentIndex(5); loadSaveWords(); });

        connect(ui->pushButton_18, &QPushButton::clicked, [this]()
                { ui->stackedWidget->setCurrentIndex(6); loadInformation(); });
        connect(ui->pushButton_19, &QPushButton::clicked, [this]()
                { ui->stackedWidget->setCurrentIndex(7); searchByChinese(); });
        connect(ui->pushButton_20, &QPushButton::clicked, [this]()
                { ui->stackedWidget->setCurrentIndex(8); searchByEnglish(); });
        connect(ui->pushButton_25, &QPushButton::clicked, [this]()
                { ui->stackedWidget->setCurrentIndex(9); initQuickMode(); });
        // 连接comboBox信号
        connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &MainWindow::onDatabaseComboBoxChanged);

        // 初始化数据库（根据comboBox当前选项）
        int dbIndex = ui->comboBox->currentIndex();
        if (dbIndex < 0)
                dbIndex = 0; // 未选择时默认0
        onDatabaseComboBoxChanged(dbIndex);
        // 初始化闹钟相关变量
        alarmTimer = new QTimer(this);
        alarmActive = false;
        connect(alarmTimer, &QTimer::timeout, this, &MainWindow::checkAlarm);

        // 连接设置闹钟按钮
        connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::setClock);

        // 时钟显示
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, [this]()
                { ui->datetimeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")); });
        timer->start(1000);

        // 选择模式
        currentWordId = -1;
        connect(ui->choice_1, &QPushButton::clicked, this, &MainWindow::onChoiceButtonClicked);
        connect(ui->choice_2, &QPushButton::clicked, this, &MainWindow::onChoiceButtonClicked);
        connect(ui->choice_3, &QPushButton::clicked, this, &MainWindow::onChoiceButtonClicked);
        connect(ui->choice_4, &QPushButton::clicked, this, &MainWindow::onChoiceButtonClicked);
}

void MainWindow::onDatabaseComboBoxChanged(int index)
{
    QString dbName;
    // 假设导入选项是最后一个
    int importIndex = ui->comboBox->count() - 1;
    if (index == 0) {
        dbName = "wordle02";
    } else if (index == 1) {
        dbName = "note";
    } else if (index == importIndex) {
        // 弹出文件选择对话框
        QString fileName = QFileDialog::getOpenFileName(
            this,
            "选择数据库文件",
            "",
            "SQLite数据库 (*.db *.db3 *.sqlite *.sqlite3);;所有文件 (*)"
        );        if (!fileName.isEmpty()) {
            dbName = fileName;
        } else {
            // 用户取消选择，恢复到上一个选项
            ui->comboBox->setCurrentIndex(0);
            dbName = "wordle02";
        }
    } else {
        dbName = "wordle02"; // 默认
    }

    db.close();
    db = QSqlDatabase::addDatabase("QSQLITE", "main_connection");
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database switched to:" << dbName;
    }
}

MainWindow::~MainWindow()
{
        delete ui;
}
