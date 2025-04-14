/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   photoview
* @brief         Photoview.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-16
*******************************************************************/
import QtQuick 2.0
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import "../helpbutton"

Item {
    property string currtenPhotoPath
    visible: false
    id: photoview
    z: 20

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/album.png"
        appName: "相册"
        appVersion: "Version 1.0.1"
        subtoolText: "1.此软件为相册应用程序，如果是在正点原子IMX6ULL开发板上跑，图片文件默认放于程序固定的文件夹/opt/src/images目录下，程序启动时会自动读取相册文件。
2.在相册播放时如果按住该图片则可以拖动，使用鼠标滚轮可缩小与放大，左右滑动可切换图片。"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本\n2.【Version 1.0.1 2020-11-24】。\n修复图片切换不流畅的问题"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }
    Component.onCompleted: {
        photoview.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: photoview
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
        id: bg
        anchors.fill: parent
        color: "#1f1e58"
    }
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        SwipeView {
            id: photoSwipeView
            anchors.fill: parent
            interactive: false
            orientation: ListView.Horizontal
            Component.onCompleted: {
                contentItem.highlightMoveDuration = 500

            }
            PhotoListView{}
            DisplayPhoto{}
            onCurrentIndexChanged : {
                photoSwipeView.currentItem.visible = true
                for (var i = 0; i < photoSwipeView.count; i++){
                    if (i !== photoSwipeView.currentIndex)
                        photoSwipeView.itemAt(i).visible = false
                }
            }
        }
    }
}
