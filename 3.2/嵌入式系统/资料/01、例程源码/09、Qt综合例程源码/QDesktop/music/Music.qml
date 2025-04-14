/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   music
* @brief         Music.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-05
*******************************************************************/
import dataModel 1.0
import QtQuick.Controls 2.5
import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import QtGraphicalEffects 1.0
import QtMultimedia 5.0
import QtQuick 2.4
import QtQuick 2.0
import "../helpbutton"

Item {
    visible: false
    id: music

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/music.png"
        appName: "音乐"
        appVersion: "Version 1.0.1"
        subtoolText: "1.此软件为音乐播放器，请将音乐文件、歌词文件按如0.mp3,0.lrc，歌词和音乐文件名字对应程序会自动读出里面的写真文件、歌曲名和歌手名。歌词要是“UFT-8”编码格式。
2.请使用MP3tag软件，准备好一张jpg 最好是50*50以上大小的写真图片，与对应的mp3歌曲，可自行百度mp3tag的使用方法，把写真图片插入到歌曲里，软件才会读出写真图片，同时可修改歌手或者歌曲名称！
3.注意程序只定义了20首歌曲的路径，0.mp3~20.mp3!具体请看程序，程序仅供学习与参考
4.如果是在正点原子IMX6ULL开发板上运行，歌词歌曲默认放在/opt/src/目录下！
5.如果是Windows下编译，64位的PC建议Qt Creator选择64位的编译器。"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本\n2.【Version 1.0.1 2020-12-01】。\n修复在Windows某些Qt Creator版本编译运行不了的问题"
    }

    function songsInit(){
        plm.add(appCurrtentDir)
    }
    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }
    Component.onCompleted: {
        music.x = parent.width
        songsInit()
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: music
                x: 0
            }
        }
        transitions: Transition {
            NumberAnimation {
                property: "x"
                easing.type: Easing.InOutQuart
                duration: 500
            }
        }
    }

    Rectangle {
        id: playListbg
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: sprogress.top
        color: "transparent"
    }
    Rectangle {
        id: songListbg
        anchors.top: playListbg.top
        anchors.bottom: playListbg.bottom
        anchors.left: playListbg.left
        anchors.right: playListbg.right
        anchors.bottomMargin: 10
        anchors.topMargin: 50
        color: "transparent"
    }
    Rectangle {
        id: topToolWidget
        anchors.top: playListbg.top
        width: playListbg.width
        height: 40
        anchors.horizontalCenter: playListbg.horizontalCenter
        color: "#30141414"
    }
    Button {
        id: btnListText
        anchors.centerIn: topToolWidget
        anchors.horizontalCenterOffset: - 50
        width: 80
        height: 40
        Text {
            id: listText
            text: qsTr("播放列表")
            color: "#27e0fb"
            font.pointSize: 10
            anchors.centerIn: parent
        }
        Rectangle{
            width: btnListText.width
            height: playList.visible ? 1 : 0
            anchors.bottom: btnListText.bottom
            color: "#27e0fb"
        }
        style: ButtonStyle {
            background: Rectangle{
                anchors.fill: parent
                color: "transparent"
            }
        }
        onClicked: {
            playList.visible = true
            lyricMask.visible = false
            lyric.visible = false
        }
    }

    Button {
        id: btnLyricText
        anchors.centerIn: topToolWidget
        anchors.horizontalCenterOffset: 50
        width: 80
        height: 40
        Text {
            id: lyricText
            text: qsTr("歌词")
            color: "#27e0fb"
            font.pointSize: 10
            anchors.centerIn: parent
        }
        Rectangle{
            width: btnLyricText.width
            height: playList.visible ? 0 : 1
            anchors.bottom: btnLyricText.bottom
            color: "#27e0fb"
        }
        style: ButtonStyle {
            background: Rectangle{
                anchors.fill: parent
                color: "transparent"
            }
        }
        onClicked: {
            playList.visible = false
            lyricMask.visible = true
            lyric.visible = true
        }
    }
    Rectangle {
        id: bg1
        anchors.top: sprogress.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: -2
        anchors.rightMargin: -2
        color: "transparent"
    }

    Audio {
        id: ado
        source: ""
        volume: 0.5
        autoPlay: myMusicstate
        onAutoPlayChanged: {
            if(myMusicstate)
                ado.play()
            else
                ado.pause()
        }
        onSourceChanged: {
            lm.setPathofSong(source, appCurrtentDir);
            //console.log( ado.volume);
        }
        onPositionChanged: {
            sprogress.maximumValue = duration;
            if(!sprogress.pressed)
                sprogress.value = position;
        }
        onPlaybackStateChanged: {
            switch (playbackState) {
            case Audio.PlayingState:
                btnplay.checked = true;
                break;
            case Audio.PausedState:
            case Audio.StoppedState:
                btnplay.checked = false;
                break;
            }
        }
        onStatusChanged: {
            switch (status) {
            case Audio.NoMedia:
                //console.log("status:nomedia");
                break;
            case Audio.Loading:
                //console.log("status:loading");
                break;
            case Audio.Loaded:
                //console.log("status:loaded");
                sprogress.maximumValue = duration;
                if (metaData.title) {
                    plm.setCurrentTitle(metaData.title);
                }
                if (metaData.albumArtist) {
                    plm.setCurrentAuthor(metaData.albumArtist);
                }
                break;
            case Audio.Buffering:
                //console.log("status:buffering");
                break;
            case Audio.Stalled:
                //console.log("status:stalled");
                break;
            case Audio.Buffered:
                //console.log("status:buffered");
                break;
            case Audio.InvalidMedia:
                //console.log("status:invalid media");
                switch (error) {
                case Audio.FormatError:
                    ttitle.text = qsTr("需要安装解码器");
                    break;
                case Audio.ResourceError:
                    ttitle.text = qsTr("文件错误");
                    break;
                case Audio.NetworkError:
                    ttitle.text = qsTr("网络错误");
                    break;
                case Audio.AccessDenied:
                    ttitle.text = qsTr("权限不足");
                    break;
                case Audio.ServiceMissing:
                    ttitle.text = qsTr("无法启用多媒体服务");
                    break;
                }
                break;
            case Audio.EndOfMedia:
                //console.log("status:end of media");
                lm.currentIndex = 0;
                sprogress.value = 0;
                switch (btnloopMode.loopMode) {
                case 1:
                    ado.play();
                    break;
                case 2:
                    plm.currentIndex ++;
                    break;
                case 3:
                    plm.randomIndex();
                    break;
                }
                break;
            }
        }
    }

    PlayListModel {
        id: plm
        onCurrentIndexChanged: {
            ado.source = getcurrentPath();
            playList.currentIndex = currentIndex;
        }
    }

    LyricModel {
        id: lm
        onCurrentIndexChanged: {
            lyric.currentIndex = currentIndex;
        }
    }

    ListView {
        id: lyric
        visible: false
        //anchors.left: parent.horizontalCenter
        //anchors.leftMargin: -80
        anchors.left: albumImage.right
        anchors.right: parent.right
        anchors.top: topToolWidget.bottom
        anchors.bottom: bg1.top
        clip: true
        spacing: 27
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: parent.height >= 450 ? 40 : 10
        preferredHighlightEnd:  parent.height >= 450 ? 150 : 30
        highlight: Rectangle {
            color: Qt.rgba(0, 0, 0, 0)
            Behavior on y {
                SmoothedAnimation {
                    duration: 300
                }
            }
        }
        model: lm
        delegate: Rectangle {
            width: parent.width
            height: 13
            color: Qt.rgba(0,0,0,0)
            Text {
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                text: "   " + textLine
                color: parent.ListView.isCurrentItem ? "#27e0fb" : "white"
                font.pointSize: parent.ListView.isCurrentItem ? 20 : 13
                font.bold: parent.ListView.isCurrentItem
            }
        }
    }

    Image {
        id: imglyricMask
        visible: false
        anchors.fill: parent
        source: "qrc:/music/images/lyricmask.png"
    }

    OpacityMask {
        id: lyricMask
        visible: false
        anchors.fill: lyric
        source: lyric
        maskSource: imglyricMask
    }

    Slider {
        id: sprogress
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: btnplay.top
        anchors.bottomMargin: 5
        updateValueWhileDragging: false
        stepSize: 30
        property bool handled: false
        onPressedChanged: {
            handled = true;
        }
        onValueChanged: {
            if (handled && ado.seekable) {
                lm.findIndex(value);
                ado.seek(value);
                ado.play();
                handled = false;
            } else {
                lm.getIndex(value);
            }
        }
        style: SliderStyle {
            groove: Rectangle {
                width: control.width
                height: 1
                radius: 1
                color: "#303030"
                Rectangle {
                    width: styleData.handlePosition
                    height: 1
                    color: "#27e0fb"
                    radius: 1
                }
            }
            handle: Rectangle {
                width: 30
                height: 30
                color: "transparent"
            }
        }
    }

    ListView {
        id: playList
        visible: true
        anchors.fill: songListbg
        anchors.bottom: songListbg.bottom
        clip: true
        spacing: 10
        ScrollBar.vertical: ScrollBar {
            id: scrollBar
            width: 5
            onActiveChanged: {
                active = true;
            }
            Component.onCompleted: {
                scrollBar.active = true;
            }
            contentItem: Rectangle{
                implicitWidth: 1
                implicitHeight: 100
                radius: 1
                color: scrollBar.hovered ? Qt.rgba(46, 46, 46, 0.2) : Qt.rgba(46, 46, 46, 0.1)
            }
        }
        model: plm
        delegate: Rectangle {
            id: itembg
            width: parent.width -10
            height: playList.currentIndex == index && ado.playbackState == Audio.PlayingState ? 60 : 45
            color: playList.currentIndex == index && ado.playbackState
                   == Audio.PlayingState ? Qt.rgba(46, 46, 46, 0.1) : "transparent"
            Image {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                width: itembg.height
                height: itembg.height
                id: album
                source: ado.playbackState == Audio.PlayingState && playList.currentIndex == index  ?
                        "file:///" + appCurrtentDir + "/src/artist/" +index +".jpg" : ""
            }

            Text {
                id: songsname
                width: itembg.width - 50
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: album.right
                verticalAlignment: Text.AlignVCenter
                text:  "  " + (index + 1 )+ " " + title
                elide: Text.ElideRight
                color: parent.ListView.isCurrentItem && ado.playbackState == Audio.PlayingState ? "#27e0fb" : "white"
                font.pointSize: 13
                font.bold: parent.ListView.isCurrentItem && ado.playbackState == Audio.PlayingState
            }
            Text {
                id: songsauthor
                visible: true
                width: 200
                height: 15
                anchors.bottom: parent.bottom
                anchors.left: album.right
                verticalAlignment: Text.AlignVCenter;
                text: "     " + author
                elide: Text.ElideRight
                color: parent.ListView.isCurrentItem && ado.playbackState == Audio.PlayingState ? "#27e0fb" : "gray"
                font.pointSize: 10
                font.bold: parent.ListView.isCurrentItem
            }
            MouseArea {
                id: mouserArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    plm.currentIndex = index
                    if (ado.playbackState != Audio.PlayingState)
                        myMusicstate = true
                }
            }
            Button {
                id: itembtn
                anchors.right: parent.right
                anchors.verticalCenter: itembg.verticalCenter
                width: itembg.height
                height: itembg.height
                onClicked: {
                    plm.currentIndex = index
                    if (ado.playbackState != Audio.PlayingState)
                        myMusicstate = true
                    else
                        myMusicstate = false

                }
                style: ButtonStyle {
                    background: Rectangle {
                        width: Control.width
                        height: Control.height
                        radius: 3
                        color: Qt.rgba(0,0,0,0)
                        Image {
                            id: itemImage
                            width: parent.height - 20
                            height: parent.height - 20
                            anchors.centerIn: parent
                            source:  playList.currentIndex != index || ado.playbackState != Audio.PlayingState
                                     ? "qrc:/music/images/btn_play.png" : "qrc:/music/images/btn_pause.png"
                            opacity: 0.8
                        }
                    }
                }
            }

        }
    }

    Image {
        visible: lyricMask.visible
        id: albumImage
        source: "qrc:/music/images/cd.png"
        width: smallScreen ?  70 : 130
        height: smallScreen ?  70 : 130
        antialiasing: true
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: smallScreen ? 130 : 150
        anchors.bottom: parent.verticalCenter
        anchors.bottomMargin: smallScreen ? -15 : 0
    }

    RotationAnimator {
        id: anmimgalbum
        target: albumImage
        from: 0
        to: 360
        duration: 50000
        loops: Animation.Infinite
        running: ado.playbackState == Audio.PlayingState &&  lyricMask.visible
        onRunningChanged: {
            if (running === false) {
                from = albumImage.rotation;
                to = from + 360;
            }
        }
    }

    Text {
        id: titleText
        anchors.top: albumImage.bottom
        anchors.left: albumImage.left
        anchors.topMargin: smallScreen ? 10 : 20
        visible: lyricMask.visible
        text: ado.playbackState == Audio.PlayingState ? "歌 名：" + plm.getcurrentTitle() : ""
        color: "#aaffffff"
        font.pixelSize: 13
    }

    Text {
        id: singerText
        anchors.top: titleText.bottom
        anchors.left: albumImage.left
        anchors.topMargin: 10
        visible: lyricMask.visible
        text: ado.playbackState == Audio.PlayingState ? "演 唱：" + plm.getcurrentAuthor() : ""
        color: "#aaffffff"
        font.pixelSize: 13
    }

    Text{
        anchors.left:  btnforward.right
        anchors.leftMargin: 20
        anchors.verticalCenter: btnplay.verticalCenter
        text: currentMusicTime(ado.position) + "/" + currentMusicTime(ado.duration)
        color: "#27e0fb"
        font.pointSize: 13
        font.bold: true
    }

    function currentMusicTime(time){
        var sec = Math.floor(time / 1000);
        var hours = Math.floor(sec / 3600);
        var minutes = Math.floor((sec - hours * 3600) / 60);
        var seconds = sec - hours * 3600 - minutes * 60;
        var hh, mm, ss;
        if(hours.toString().length < 2)
            hh = "0" + hours.toString();
        else
            hh = hours.toString();
        if(minutes.toString().length < 2)
            mm="0" + minutes.toString();
        else
            mm = minutes.toString();
        if(seconds.toString().length < 2)
            ss = "0" + seconds.toString();
        else
            ss = seconds.toString();
        return /*hh+":"*/ + mm + ":" + ss
    }

    Button {
        id: btnlove
        anchors.right: btnloopMode.left
        anchors.rightMargin: 20
        anchors.verticalCenter: btnplay.verticalCenter
        style: ButtonStyle {
            background: Image {
                source: btnlove.checked ? "qrc:/music/images/btn_favorite_no.png"
                                        : "qrc:/music/images/btn_favorite_yes.png"
            }
        }
        onClicked: btnlove.checked = !btnlove.checked
    }

    Button {
        id: btnplay
        anchors.horizontalCenter : parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        width: 42
        height: 42
        checkable: true
        onClicked: {
            if (playList.currentIndex != -1) {
                (ado.playbackState === Audio.PlayingState) ? ado.pause() : ado.play();
            }
            ado.playbackState != Audio.PlayingState ? myMusicstate = false : myMusicstate = true
        }
        style: ButtonStyle {
            background: Image {
                id: imgplay
                source: ado.hasAudio && control.checked ? "qrc:/music/images/btn_pause.png"
                                                        : "qrc:/music/images/btn_play.png"
                opacity: control.hovered ? 0.7 : 1.0;
            }
        }
    }

    Button {
        id: btnbackward
        anchors.verticalCenter: btnplay.verticalCenter
        anchors.right: btnplay.left
        anchors.rightMargin: 20
        width: 32
        height: 32
        onClicked: {
            switch (btnloopMode.loopMode) {
            case 0:
            case 1:
            case 2:
                plm.currentIndex --
                break;
            case 3:
                plm.randomIndex();
                break;
            }
        }
        style: ButtonStyle {
            background: Image {
                id: imgbackward
                source: "qrc:/music/images/btn_previous.png"
                opacity: control.hovered ? 0.7 : 1.0;
            }
        }
    }

    Button {
        id: btnforward
        anchors.verticalCenter: btnplay.verticalCenter
        anchors.left: btnplay.right
        anchors.leftMargin: 20
        width: 32
        height: 32
        onClicked: {
            if (ado.hasAudio)
                switch (btnloopMode.loopMode) {
                case 0:
                case 1:
                case 2:
                    plm.currentIndex ++
                    break;
                case 3:
                    plm.randomIndex();
                    break;
                }
        }
        style: ButtonStyle {
            background: Image {
                id: imgforward
                source: "qrc:/music/images/btn_next.png"
                opacity: control.hovered ? 0.7 : 1.0;
            }
        }
    }

    Button {
        id: btnloopMode
        height: 25
        width: 25
        anchors.verticalCenter: btnplay.verticalCenter
        anchors.right: btnbackward.left
        anchors.rightMargin: 20
        property int loopMode: 2
        onLoopModeChanged: {
            switch (loopMode) {
            case 0:
                tooltip = qsTr("单曲播放")
                break;
            case 1:
                tooltip = qsTr("单曲循环")
                break;
            case 2:
                tooltip = qsTr("顺序播放");
                break;
            case 3:
                tooltip = qsTr("随机播放");
                break;
            default:
                loopMode = 0;
            }
        }
        onClicked: loopMode++
        tooltip: qsTr("单曲播放")
        style: ButtonStyle {
            background: Rectangle {
                width: 25
                height: 25
                radius: 3
                color: Qt.rgba(0,0,0,0)
                Image {
                    id: imgloopMode
                    anchors.fill: parent
                    anchors.centerIn: parent
                    source: {
                        switch (control.loopMode) {
                        case 1:
                            return "qrc:/music/images/btn_listscircle_single.png"
                        case 2:
                            return "qrc:/music/images/btn_listjump.png"
                        case 3:
                            return "qrc:/music/images/btn_listrandom.png"
                        default:
                            return "qrc:/music/images/btn_listsingle.png"
                        }
                    }
                }
            }
        }
    }
}


