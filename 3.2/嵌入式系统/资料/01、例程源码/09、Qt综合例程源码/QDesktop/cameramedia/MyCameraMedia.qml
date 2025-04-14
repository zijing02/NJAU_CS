/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   iotest
* @brief         Iotest.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-20
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import MyCameraMedia 1.0
import "../helpbutton"

Item {
    visible: false
    id: camera

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/camera.png"
        appName: "ov5640摄像头/媒体"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件是测试ALPHA/Mini底板ov5640的摄像头和播放视频软件，点击对应项即开始/停止"
        versionText: "1.【Version 1.0.0 2020-11-18】。\n初始版本"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }
    Component.onCompleted: {
        camera.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: camera
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

    CameraMedia {
        id: myCameraMedia
    }
    Flickable {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: parent.height +20
        Dialog {
            id: startDialog
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
            background: Rectangle {
                anchors.fill: parent
                color: "#1f1e58"
            }
            MouseArea {
                anchors.fill: parent
                onClicked:  {
                    startDialog.close()
                    if (key0.checked)
                        myCameraMedia.setCameraState(false)
                    if (key1.checked)
                        myCameraMedia.setMediaState(false)
                }
            }
        }
        Button {
            id: key0
            width: 300
            height: 60
            checkable: true
            checked: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.verticalCenter
            anchors.bottomMargin: 30
            Text {
                text: qsTr("ov5640摄像头")
                color: "white"
                anchors.centerIn: parent
                font.pixelSize: 20
                font.bold: true
            }
            style: ButtonStyle {
                background: Rectangle {
                    id: button1
                    anchors.fill: parent
                    color: "#0195de"
                    radius: 8
                }
            }
            onClicked: {
                key0.checked = !key0.checked
                myCameraMedia.setCameraState(true)
                startDialog.open()
            }
        }
        Button {
            id: key1
            width: 300
            height: 60
            checkable: true
            checked: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.verticalCenter
            anchors.topMargin: 30
            Text {
                text: qsTr("播放视频")
                color: "white"
                anchors.centerIn: parent
                font.pixelSize: 20
                font.bold: true
            }
            style: ButtonStyle {
                background: Rectangle {
                    anchors.fill: parent
                    color: "#0195de"
                    radius: 8
                }
            }
            onClicked: {
                key1.checked = !key1.checked
                myCameraMedia.setMediaState(true)
                startDialog.open()
            }
        }
    }
}
