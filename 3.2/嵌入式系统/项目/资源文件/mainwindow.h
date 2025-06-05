#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QTimer>         // 用于定时器功能
#include <QDateTime>      // 用于获取当前日期时间
#include <QSqlDatabase>   // 添加数据库支持
#include <QSqlQuery>      // 添加SQL查询支持
#include <QSqlError>      // 添加SQL错误处理
#include <QTableWidget>   // 添加表格支持
#include <QMediaPlayer>   // 添加媒体播放支持
#include <QMediaPlaylist> // 添加播放列表支持
#include <QFileDialog>    // 添加文件对话框支持
#include <QHeaderView>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QRandomGenerator> // 随机函数
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMediaMetaData>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QListWidget>
#include <QInputDialog>
#include <QTimeEdit>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_14_clicked();
    void on_pushButton_27_clicked();
    void on_pushButton_28_clicked();
    void on_pushButton_29_clicked();
    void on_pushButton_30_clicked();
    void onDatabaseComboBoxChanged(int index);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;           // 数据库对象
    QMediaPlayer *musicPlayer; // 添加音乐播放器指针
    QMediaPlaylist *playlist;  // 添加播放列表指针
    /* 选择/填空/速刷模式 */
    void initChoiceMode();  // 初始化选择模式
    void loadChoiceWords(); // 加载选择模式
    int correctAnswerIndex; // 用于存储正确答案的位置(1-4)
    QSet<QString> displayedWords;
    QVector<QPair<QString, QString>> allWords; // 存储所有单词数据
    QVector<QString> allMeanings;              // 存储所有中文解释(用于生成干扰项)
    int currentWordId;     // 当前单词ID
    bool hasRecorded; // 是否已记录本次错误
    bool needRedisplay; // 是否需要重复出现
    QTimer *alarmTimer;  // 闹钟定时器
    QTime alarmTime;     // 存储闹钟时间
    bool alarmActive;       //闹钟是否激活
    QFile BeepFile;       //蜂鸣器路径
    bool alarmUseBeep = true;         // true: 使用蜂鸣器, false: 使用音乐
    QString alarmMusicPath;     //闹钟音乐路径
    QMediaPlayer *alarmPlayer = nullptr;      //闹钟音乐播放器

    void updateErrorCount();
    void toggleSaveStatus(QPushButton *button);
    void updateCollectButton(QPushButton *button);
    void resetButtonColors();
    void highlightAnswer(QPushButton *button, bool isCorrect);
    void onChoiceButtonClicked();
    QPushButton *getButtonByIndex(int index);

    void initBlankMode();
    void loadBlankWords();
    void highlightBlankAnswer(QLineEdit *lineEdit, bool isCorrect);

    void initQuickMode();
    void loadQuickWords();

    void loadInformation();                                                       // 加载所有信息
    void loadWrongWords();                                                        // 加载错题单词
    void changeWrongTimes(int row, int column, QTableWidget *tableWidget);        // 修改错误次数
    void loadSaveWords();                                                         // 加载收藏单词
    void changeSaveTime(int row, int column, QTableWidget *tableWidget, int pos); // 修改收藏
    void searchByChinese();                                                       // 中文搜索
    void performChineseSearch(const QString &searchText, QTableWidget *tableWidget);
    void searchByEnglish(); // 英文搜索
    void performEnglishSearch(const QString &searchText, QTableWidget *tableWidget);
    void loadMusic(); // 加载音乐

    void setClock();  // 设置闹钟按钮点击事件
    void checkAlarm();               // 检查闹钟时间
    void handleAlarmResponse(int result); // 处理闹钟响应

    void setBeepState();
    bool getBeepState();

};
#endif // MAINWINDOW_H
