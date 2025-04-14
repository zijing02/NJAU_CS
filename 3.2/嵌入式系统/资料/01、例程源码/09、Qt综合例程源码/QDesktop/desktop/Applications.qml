/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   desktop
* @brief         Applications.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-01
*******************************************************************/
import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    property real iconsOpacity: 1
    visible: false
    id: apps
    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }
    Component.onCompleted: {
        apps.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: apps
                y: 480
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

    PageIndicator {
        id: indicator

        count: swipeView_app.count
        currentIndex: swipeView_app.currentIndex

        anchors.bottom: swipeView_app.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    SwipeView {
        id: swipeView_app
        anchors.fill: parent
        currentIndex: 0

        Item {
            ListModel {
                id: appModel2
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                function getIcon(index, id) {
                    var data = [
                                ["androidstyleicons/music.png"],
                                ["androidstyleicons/player.png"],
                                ["androidstyleicons/weather.png"],
                                ["androidstyleicons/radio.png"],
                                ["androidstyleicons/calculator.png"],
                                ["androidstyleicons/tcpserver.png"],
                                ["androidstyleicons/tcpclient.png"],
                                ["androidstyleicons/udpchat.png"],
                                ["androidstyleicons/album.png"],
                                ["androidstyleicons/fileview.png"],
                                ["androidstyleicons/aflex.png"],
                                ["androidstyleicons/iotest.png"],
                            ]
                    return data[index][0];
                }

                function getAppName(index, id) {
                    var data = [
//                                ["Music"],
//                                ["Media"],
//                                ["Weather"],
//                                ["Clock"],
//                                ["Calculator"],
//                                ["TCP Server"],
//                                ["Tcp Client"],
//                                ["UDP Chat"],
//                                ["Album"],
//                                ["File"],
//                                ["Aflex"],
//                                ["Iotest"],
                                ["音乐"],
                                ["媒体播放器"],
                                ["天气"],
                                ["收音机FM"],
                                ["计算器"],
                                ["TCP 服务器"],
                                ["Tcp 客户端"],
                                ["UDP 聊天"],
                                ["相册"],
                                ["文件夹"],
                                ["电压测量"],
                                ["按键测试"],
                            ]
                    return data[index][0];
                }
            }

            GridView {
                id: app_view2
                visible: true
                anchors.top: parent.top

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                focus: true
                interactive: false
                cellWidth: app_view2.width / 4
                cellHeight: (app_view2.height) / 3
                model: appModel2
                delegate: delegateThumbnail2

            }
            Component {
                id: delegateThumbnail2
                Rectangle {
                    width: app_view2.width / 4
                    height: app_view2.cellHeight
                    color: "transparent"

                    Rectangle {
                        width: 84
                        height: 84
                        radius: 8
                        color: "transparent"
                        anchors.centerIn: parent
                        Image {
                            id: browse_img
                            antialiasing: true
                            source: app_view2.model.getIcon(index, 0)
                            anchors.fill: parent
                            fillMode: Image.Stretch
                            opacity: app_view2.currentIndex == index ? iconsOpacity : 1
                        }
                        Text {
                            anchors.top: browse_img.bottom
                            anchors.topMargin: 5
                            anchors.horizontalCenter: parent.horizontalCenter
                            id: appName
                            text: qsTr(app_view2.model.getAppName(index, 0))
                            color: "white"
                            font.pixelSize: 15
                            font.bold: true
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                app_view2.currentIndex = index
                                if (index + 1 >= 3)
                                    mainSwipeView.currentIndex = index + 2
                                else
                                    mainSwipeView.currentIndex = index + 1
                                backFlag = false
                            }
                            onEntered: {
                                app_view2.currentIndex = index
                                iconsOpacity = 0.5
                            }
                            onPressed:{
                                iconsOpacity = 0.8
                                app_view2.currentIndex = index
                            }
                            onReleased:{
                                iconsOpacity = 1
                            }
                            onExited: iconsOpacity = 1
                        }
                    }
                }
            }
        }
        Item {
            ListModel {
                id: appModel3
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                ListElement {name: ""}
                function getIcon2(index, id) {
                    var data = [
                                ["androidstyleicons/sensor.png"],
                                ["androidstyleicons/wireless.png"],
                                ["androidstyleicons/cube.png"],
                                ["androidstyleicons/system.png"],
                                ["androidstyleicons/camera.png"],
                                ["androidstyleicons/time.png"],
                            ]
                    return data[index][0];
                }

                function getAppName2(index, id) {
                    var data = [
//                                ["Sensor"],
//                                ["Wireless"],
//                                ["3DCube"],
//                                ["System"],
//                                ["Camera"],
                                ["传感器"],
                                ["WIFI"],
                                ["3D立方体"],
                                ["系统信息"],
                                ["倒车影像"],
                                ["闹钟"],
                            ]
                    return data[index][0];
                }
            }

            GridView {
                id: app_view3
                visible: true
                anchors.top: parent.top

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                focus: true
                interactive: false
                cellWidth: app_view3.width / 4
                cellHeight: (app_view3.height) / 3
                model: appModel3
                delegate: delegateThumbnail3

            }
            Component {
                id: delegateThumbnail3
                Rectangle {
                    width: app_view3.width / 4
                    height: app_view3.cellHeight
                    color: "transparent"

                    Rectangle {
                        width: 84
                        height: 84
                        radius: 8
                        color: "transparent"
                        anchors.centerIn: parent
                        Image {
                            id: browse_img
                            antialiasing: true
                            source: app_view3.model.getIcon2(index, 0)
                            anchors.fill: parent
                            fillMode: Image.Stretch
                            opacity: app_view3.currentIndex == index ? iconsOpacity : 1
                        }
                        Text {
                            anchors.top: browse_img.bottom
                            anchors.topMargin: 5
                            anchors.horizontalCenter: parent.horizontalCenter
                            id: appName
                            text: qsTr(app_view3.model.getAppName2(index, 0))
                            color: "white"
                            font.pixelSize: 15
                            font.bold: true
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                app_view3.currentIndex = index
                                mainSwipeView.currentIndex = index + 14
                                backFlag = false
                            }
                            onEntered: {
                                app_view3.currentIndex = index
                                iconsOpacity = 0.5
                            }
                            onPressed:{
                                iconsOpacity = 0.8
                                app_view3.currentIndex = index
                            }
                            onReleased:{
                                iconsOpacity = 1
                            }
                            onExited: iconsOpacity = 1
                        }
                    }
                }
            }
        }
    }
}
