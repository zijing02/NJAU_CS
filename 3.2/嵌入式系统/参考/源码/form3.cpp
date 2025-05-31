#include "form3.h"
#include "ui_form3.h"

extern vector<QStringList> words;
extern int idx;
Form3::Form3(QWidget *parent): QWidget(parent), ui(new Ui::Form3)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    player->setPlaylist(playlist);

    connect(player,SIGNAL(positionChanged(qint64)),
            this, SLOT(onPositionChanged(qint64)));

    connect(player,SIGNAL(durationChanged(qint64)),
            this, SLOT(onDurationChanged(qint64)));

    connect(playlist,SIGNAL(currentIndexChanged(int)),
            this, SLOT(onPlaylistChanged(int)));
}
Form3::~Form3(){delete ui;}

void Form3::onDurationChanged(qint64 duration)
{     ui->process->setMaximum(duration);

     int secs=duration/1000;
     int mins=secs/60;
     secs=secs % 60;
     durationTime=QString::asprintf("%d:%d",mins,secs);
     ui->time->setText(positionTime+"/"+durationTime);
}

void Form3::onPlaylistChanged(int position)
{
    ui->listWidget->setCurrentRow(position);
    QListWidgetItem  *item=ui->listWidget->currentItem();
    if (item)
        ui->label->setText(item->text());
}



void Form3::onPositionChanged(qint64 position)
{
    if (ui->process->isSliderDown())
        return;

    ui->process->setSliderPosition(position);//

    int secs=position/1000;
    int mins=secs/60;
    secs=secs % 60;
    positionTime=QString::asprintf("%d:%d",mins,secs);
    ui->time->setText(positionTime+"/"+durationTime);
}


void Form3::on_bt4_clicked()
{
    QString curPath=QDir::homePath();//获取系统当前目录
    QString dlgTitle="选择音频文件"; //对话框标题
    QString filter="音频文件(*.mp3 *.wav *.wma);;mp3文件(*.mp3);;wav文件(*.wav);;wma文件(*.wma);;所有文件(*.*)"; //文件过滤器
    QStringList fileList=QFileDialog::getOpenFileNames(this,dlgTitle,curPath,filter);

    if (fileList.count()<1)
        return;

    for (int i=0; i<fileList.count();i++)
    {
       QString aFile=fileList.at(i);
       playlist->addMedia(QUrl::fromLocalFile(aFile));//添加文件

       QFileInfo   fileInfo(aFile);
       ui->listWidget->addItem(fileInfo.fileName());//添加到界面文件列表
    }

    if (player->state()!=QMediaPlayer::PlayingState)
        playlist->setCurrentIndex(0);
    player->play();
}

void Form3::on_bt1_clicked()
{
    if(!pause_flag){
        if(playlist->currentIndex() < 0) playlist->setCurrentIndex(0);
        player->play();
        pause_flag = true;
    }
    else{
        player->pause();
        pause_flag = false;
    }
}

void Form3::on_listWidget_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    playlist->setCurrentIndex(row);
    player->play();
}

void Form3::on_bt6_clicked()
{
    playlist->clear();
    ui->listWidget->clear();
    player->stop();
}

void Form3::on_volumn_valueChanged(int value)
{
    player->setVolume(value);
}

void Form3::on_process_valueChanged(int value)
{
    player->setPosition(value);
}

void Form3::on_bt2_clicked()
{
    playlist->previous();
}


void Form3::on_bt3_clicked()
{
    playlist->next();
}
