#include "mainwindow.h"

void MainWindow::loadMusic()
{
    QWidget *page3 = ui->stackedWidget->widget(3);

    // 初始化音乐播放器
    QMediaPlayer *player = new QMediaPlayer(this);
    QMediaPlaylist *playlist = new QMediaPlaylist(player);
    player->setPlaylist(playlist);

    // 创建界面元素
    QVBoxLayout *mainLayout = new QVBoxLayout(page3);

    // 当前播放标签
    QLabel *currentSongLabel = new QLabel("当前未播放", page3);
    currentSongLabel->setAlignment(Qt::AlignCenter);
    currentSongLabel->setStyleSheet("font-size: 16px; font-weight: bold;");

    // 播放列表显示
    QListWidget *playlistWidget = new QListWidget(page3);
    playlistWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    playlistWidget->setMaximumWidth(650);

    // 进度条
    QSlider *progressSlider = new QSlider(Qt::Horizontal, page3);
    progressSlider->setRange(0, 100);
    progressSlider->setMaximumWidth(650);

    // 时间显示
    QLabel *timeLabel = new QLabel("00:00 / 00:00", page3);
    timeLabel->setAlignment(Qt::AlignCenter);

    // 控制按钮区域
    QHBoxLayout *controlLayout = new QHBoxLayout();

    // 音量控制
    QSlider *volumeSlider = new QSlider(Qt::Horizontal, page3);
    volumeSlider->setMaximumWidth(650);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    player->setVolume(50);

    // 添加到布局
    mainLayout->addWidget(currentSongLabel);
    mainLayout->addWidget(playlistWidget);
    mainLayout->addWidget(progressSlider);
    mainLayout->addWidget(timeLabel);
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(new QLabel("音量:", page3));
    mainLayout->addWidget(volumeSlider);

    // 获取UI中的按钮
    QPushButton *loadButton = page3->findChild<QPushButton *>("pushButton_17");
    QPushButton *prevButton = page3->findChild<QPushButton *>("pushButton_7");
    QPushButton *pauseButton = page3->findChild<QPushButton *>("pushButton_8");
    QPushButton *nextButton = page3->findChild<QPushButton *>("pushButton_9");
    QPushButton *volUpButton = page3->findChild<QPushButton *>("pushButton_15");
    QPushButton *volDownButton = page3->findChild<QPushButton *>("pushButton_16");
    QPushButton *deleteCurrentButton = page3->findChild<QPushButton *>("pushButton_21");
    QPushButton *deleteAllButton = page3->findChild<QPushButton *>("pushButton_22");
    QPushButton *forwardButton = page3->findChild<QPushButton *>("pushButton_24");   // 快进10秒
    QPushButton *backwardButton = page3->findChild<QPushButton *>("pushButton_23");  // 快退10秒
    // ...existing code...
    // 连接按钮信号
    connect(loadButton, &QPushButton::clicked, [=]()
            {
        QStringList files = QFileDialog::getOpenFileNames(
            this,
            "选择音乐文件",
            QDir::homePath(),
            "音频文件 (*.mp3 *.wav *.ogg *.flac)"
        );

        if (!files.isEmpty()) {
            for (const QString &file : files) {
                QUrl url = QUrl::fromLocalFile(file);
                // 检查是否已在播放列表中
                bool exists = false;
                for (int i = 0; i < playlist->mediaCount(); ++i) {
                    if (playlist->media(i).canonicalUrl() == url) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    playlist->addMedia(url);
                    QFileInfo fileInfo(file);
                    playlistWidget->addItem(fileInfo.fileName());
                }
            }
            // 如果当前没有播放，自动播放第一首
            if (playlist->mediaCount() > 0 && player->state() != QMediaPlayer::PlayingState) {
                playlist->setCurrentIndex(0);
                player->play();
            }
        } });

    connect(prevButton, &QPushButton::clicked, [=]()
            {
        int count = playlist->mediaCount();
        if (count == 0) return;
        int current = playlist->currentIndex();
        if (count == 1) {
            playlist->setCurrentIndex(0);
            player->setPosition(0);
            player->play();
        } else if (current <= 0) {
            playlist->setCurrentIndex(count - 1); // 跳到最后一首
            player->play();
        } else {
            playlist->previous();
        } });

    connect(nextButton, &QPushButton::clicked, [=]()
            {
        int count = playlist->mediaCount();
        if (count == 0) return;
        int current = playlist->currentIndex();
        if (count == 1) {
            playlist->setCurrentIndex(0);
            player->setPosition(0);
            player->play();
        } else if (current >= count - 1) {
            playlist->setCurrentIndex(0); // 跳到第一首
            player->play();
        } else {
            playlist->next();
        } });
    connect(pauseButton, &QPushButton::clicked, [=]()
            {
        if (player->state() == QMediaPlayer::PlayingState) {
            player->pause();
            pauseButton->setText("播放");
        } else {
            player->play();
            pauseButton->setText("暂停");
        } });

    connect(volUpButton, &QPushButton::clicked, [=]()
            {
        int volume = player->volume();
        volume += 5;
        player->setVolume(volume);
        volumeSlider->setValue(volume); });

    connect(volDownButton, &QPushButton::clicked, [=]()
            {
        int volume = player->volume();
        volume -= 5;
        player->setVolume(volume);
        volumeSlider->setValue(volume); });

    connect(deleteCurrentButton, &QPushButton::clicked, [=]() {
        int currentIndex = playlist->currentIndex();
        if (currentIndex >= 0 && currentIndex < playlist->mediaCount()) {
            playlist->removeMedia(currentIndex);
            delete playlistWidget->takeItem(currentIndex);
            // 如果还有歌曲，自动播放当前索引的下一首
            if (playlist->mediaCount() > 0) {
                int nextIndex = qMin(currentIndex, playlist->mediaCount() - 1);
                playlist->setCurrentIndex(nextIndex);
                player->play();
            } else {
                player->stop();
                currentSongLabel->setText("当前未播放");
            }
        }
    });

    connect(deleteAllButton, &QPushButton::clicked, [=]() {
        playlist->clear();
        playlistWidget->clear();
        player->stop();
        currentSongLabel->setText("当前未播放");
    });

    connect(forwardButton, &QPushButton::clicked, [=]() {
        qint64 newPos = player->position() + 10000; // 当前时间+10秒
        if (newPos > player->duration()) newPos = player->duration();
        player->setPosition(newPos);
    });

    connect(backwardButton, &QPushButton::clicked, [=]() {
        qint64 newPos = player->position() - 10000; // 当前时间-10秒
        if (newPos < 0) newPos = 0;
        player->setPosition(newPos);
    });

    connect(volumeSlider, &QSlider::valueChanged, player, &QMediaPlayer::setVolume);

    // 播放进度更新
    connect(player, &QMediaPlayer::positionChanged, [=](qint64 position)
            {
        if (!progressSlider->isSliderDown()) {
            qint64 duration = player->duration();
            if (duration > 0) {
                progressSlider->setValue(static_cast<int>(position * 100 / duration));

                QTime currentTime(0, 0);
                currentTime = currentTime.addMSecs(position);
                QTime totalTime(0, 0);
                totalTime = totalTime.addMSecs(duration);
                timeLabel->setText(currentTime.toString("mm:ss") + " / " + totalTime.toString("mm:ss"));
            }
        } });

    // 进度条拖动
    connect(progressSlider, &QSlider::sliderMoved, [=](int value)
            {
        qint64 duration = player->duration();
        if (duration > 0) {
            player->setPosition(value * duration / 100);
        } });

    // 当前曲目变化
    connect(playlist, &QMediaPlaylist::currentIndexChanged, [=](int index)
            {
        if (index >= 0 && index < playlistWidget->count()) {
            playlistWidget->setCurrentRow(index);
            currentSongLabel->setText("正在播放: " + playlistWidget->item(index)->text());
        } });

    // 双击播放列表切换歌曲
    connect(playlistWidget, &QListWidget::itemDoubleClicked, [=](QListWidgetItem *item)
            {
        int index = playlistWidget->row(item);
        playlist->setCurrentIndex(index);
        player->play(); });

    // 设置页面布局
    page3->setLayout(mainLayout);
}
