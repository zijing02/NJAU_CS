#ifndef FORM3_H
#define FORM3_H
#include <content.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Form3; }
QT_END_NAMESPACE

class Form3: public QWidget
{
    Q_OBJECT

public:
    Form3(QWidget *parent = nullptr);
    ~Form3();

private:
    QMediaPlayer    *player;//播放器
    QMediaPlaylist  *playlist;//播放列表

    QString  durationTime;//总长度
    QString  positionTime;//当前播放到位置
private slots:

    void onPlaylistChanged(int position);

    void onDurationChanged(qint64 duration);

    void onPositionChanged(qint64 position);


    void on_bt4_clicked();

    void on_bt1_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_bt6_clicked();

    void on_volumn_valueChanged(int value);

    void on_process_valueChanged(int value);

    void on_bt2_clicked();

    void on_bt3_clicked();

private:
    Ui::Form3 *ui;
};
#endif // FORM3_H
