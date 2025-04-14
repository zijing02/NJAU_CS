/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   media
* @brief         Media.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-10
*******************************************************************/
import mediaModel 1.0
import QtQuick 2.0
import QtQuick 2.2
import QtQuick 2.4
import QtMultimedia 5.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import "../helpbutton"

Item {
    visible: false
    id:mediaPlayer

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/player.png"
        appName: "媒体播放器"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件为媒体播放器，请将媒体文件放于固定位置，程序启动时自会扫描该目录下的视频文件。
2.注意根据mp157的性能，可流畅播放低于1080p的视频，如果视频质量过高，请降低视频质量再使用此软件播放！
3.注意如果您是使用格式工厂或者狸窝转换器压缩的视频需要自己转换好视频，如果遇到视频是无法播放或者黑屏，您需要注意转换格式，自行测试！"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本"
    }


    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }
    Component.onCompleted: {
        mediaPlayer.x = parent.width
        mediaInit()
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: mediaPlayer
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
    Timer {
        id: timerShow
        interval: 5000
        repeat: true
        running: true
        onTriggered: {
            timerShow.stop()
            playErea.visible = false
        }
    }

    function mediaInit(){
        plm.add(WINenv ? qsTr("C:/Users/Administrator/Desktop") : qsTr("/home/root") )
        player.stop()
    }

    MediaListModel {
        id: plm
        onCurrentIndexChanged: {
            player.source = getcurrentPath()
            playList.currentIndex = currentIndex
        }
    }

    ListView  {
        id: playList
        visible: true
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.bottom: parent.bottom
        width: parent.width
        orientation:ListView.Vertical
        clip: true
        z: 1
        spacing: 20
        model: plm
        delegate: Rectangle {
            id: itembg
            width: parent.width
            Image {
                id: listPlayState
                source: playList.currentIndex == index && player.playbackState === Audio.PlayingState
                        ? "qrc:/media/images/btn_pause.png" :"qrc:/media/images/btn_play.png"
                anchors.left: parent.left
                anchors.leftMargin: 50
                width: parent.height
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
            }
            height: playList.currentIndex == index && player.playbackState === Audio.PlayingState ? 90 : 45
            color: playList.currentIndex == index && player.playbackState === Audio.PlayingState ? Qt.rgba(46, 46, 46, 0.1) : "transparent"
            Text {
                id: mediaName
                width: parent.width
                //anchors.centerIn: parent
                //anchors.left: parent.left
                anchors.left: listPlayState.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 20
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                text:  title
                elide: Text.ElideRight
                color: parent.ListView.isCurrentItem ? "#27def9" : "white"
                font.bold: parent.ListView.isCurrentItem
                font.pixelSize: 15
            }

            MouseArea {
                id:mouserArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    plm.currentIndex = index
                    playList.visible = false
                    playErea.visible = true
                    screen.visible = true
                    //console.log(mediaName.text)
                }
            }

        }
    }

    Column{
        Rectangle{
            id: screen
            color:"black"
            width:mediaPlayer.width
            height: mediaPlayer.height
            visible: false

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    playErea.visible = true
                    if (player.hasVideo && mouseY < parent.height - 65)
                        (player.playbackState === Audio.PlayingState) ? player.pause() : player.play()
                }
            }

            MediaPlayer{
                id: player
                source: fd.fileUrl
                autoPlay: true
                volume: voice_slider.value
                onPositionChanged: {
                    if(!playPos.pressed)
                        playPos.value = position
                }

                onStatusChanged: {
                    switch (status) {
                    case Audio.EndOfMedia:
                        playPos.value = 0;
                        plm.currentIndex ++;
                        break;
                    default:
                        break;
                    }
                }

                onPlaybackStateChanged: {
                    switch (playbackState) {
                    case Audio.PlayingState:
                        btnplay.checked = true
                        timerShow.start()
                        break
                    case Audio.PausedState:
                    case Audio.StoppedState:
                        btnplay.checked = false
                        timerShow.stop()
                        if (screen.visible)
                            playErea.visible = true
                        break
                    }
                }
            }
            VideoOutput {
                anchors.fill: parent
                source: player
            }
        }
    }

    Rectangle{
        id: playErea
        color:"#90111111"
        width: parent.width
        anchors.bottom: parent.bottom
        height: 65
        visible: false
        ButtonImage{
            id: btnplay
            width: 40
            height: 40
            anchors.verticalCenter: playErea.verticalCenter
            anchors.left: playErea.left
            anchors.leftMargin: 10
            iconImage: player.hasVideo && btnplay.checked ? "qrc:/media/images/btn_pause.png" : "qrc:/media/images/btn_play.png"
            checkable: true
            onClicked: {
                if (player.hasVideo) {
                    (player.playbackState === Audio.PlayingState) ? player.pause() : player.play()
                }
            }
        }

        ButtonImage{
            width: 40
            height: 40
            id: btnlist
            anchors.left: btnplay.right
            anchors.leftMargin: 15
            anchors.verticalCenter: playErea.verticalCenter
            iconImage: "qrc:/media/images/list.png"
            onClicked: {
                playList.visible = !playList.visible
                playErea.visible = !playErea.visible
                screen.visible = false
            }
        }

        Slider{
            id:playPos
            anchors.right: parent.right
            anchors.rightMargin: 180
            height: 30
            maximumValue: player.duration
            minimumValue: 0
            anchors.verticalCenter: playErea.verticalCenter
            anchors.left: btnlist.right
            anchors.leftMargin: 20
            stepSize :1000
            updateValueWhileDragging: false
            property bool handled: false

            onPressedChanged: {
                handled = true
            }
            onValueChanged: {
                if (handled && player.seekable) {
                    player.seek(playPos.value)
                    handled = false
                }
            }

            style: SliderStyle {
                groove: Rectangle {
                    width: playPos.width
                    height: 1
                    color: "gray"
                    radius: 1
                    Rectangle {
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: styleData.handlePosition
                        color: "#27def9"
                    }
                }
                handle: Rectangle {
                    width: 30
                    height: 30
                    color: "transparent"
                }
            }
        }
        Image{
            id: imagVoice
            width: 20
            height: 20
            visible: false
            source: ""
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: playText.right
            anchors.leftMargin: 20

        }

        Slider{
            id: voice_slider
            height: 10
            value: 1
            stepSize: 0.1
            maximumValue: 1
            minimumValue: 0
            visible: false
            anchors.verticalCenter: playErea.verticalCenter
            anchors.left: imagVoice.right
            anchors.leftMargin: 10
            anchors.right: playErea.right
            anchors.rightMargin: 80

            style: SliderStyle {
                groove: Rectangle {
                    implicitHeight: 1
                    color: "gray"
                    radius: 1
                    Rectangle {
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: styleData.handlePosition
                        color: "#14aaff"
                    }
                }
                handle:
                    Image {
                    width: 25
                    height: 25
                    id: handle_image2
                    source: ""
                }
            }

        }

        Text{
            id: playText
            anchors.verticalCenter: playErea.verticalCenter
            anchors.left: playPos.right
            anchors.leftMargin: 20
            text:parent.currentTime(player.position)+"/"+parent.currentTime(player.duration)
            color: "#27def9"
        }

        function currentTime(time)
        {
            var sec= Math.floor(time/1000)
            var hours=Math.floor(sec/3600)
            var minutes=Math.floor((sec-hours*3600)/60)
            var seconds=sec-hours*3600-minutes*60
            var hh,mm,ss
            if(hours.toString().length<2)
                hh="0"+hours.toString()
            else
                hh=hours.toString()
            if(minutes.toString().length<2)
                mm="0"+minutes.toString()
            else
                mm=minutes.toString()
            if(seconds.toString().length<2)
                ss="0"+seconds.toString()
            else
                ss=seconds.toString()
            return hh+":"+mm+":"+ss
        }
    }
}

