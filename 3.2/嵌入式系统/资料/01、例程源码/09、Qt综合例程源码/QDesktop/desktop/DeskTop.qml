/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   desktop
* @brief         DeskTop.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-01
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
Item {
    visible: true
    property real iconsOpacity: 1
    ListModel {
        id: modelThumbnail
        ListElement {name: ""}
        ListElement {name: ""}
        ListElement {name: ""}
        ListElement {name: ""}
        ListElement {name: ""}
        function getIcon(index, id) {
            var data = [
                        ["androidstyleicons/music.png"],
                        ["androidstyleicons/player.png"],
                        ["androidstyleicons/apps.png"],
                        ["androidstyleicons/weather.png"],
                        ["androidstyleicons/radio.png"],
                    ]
            return data[index][0];
        }

        function getAppName(index, id) {
            var data = [
//                        ["Music"],
//                        ["Media"],
//                        [""],
//                        ["Weather"],
//                        ["Radio"],
                        ["音乐"],
                        ["媒体播放器"],
                        [""],
                        ["天气"],
                        ["收音机FM"],
                    ]
            return data[index][0];
        }
    }
    ListView {
        id: desktop_view
        height: 100
        visible: true
        anchors.top: parent.verticalCenter
        anchors.topMargin: 100
        anchors.left: parent.left
        anchors.right: parent.right
        focus: true
        interactive: false
        spacing: 0
        model: modelThumbnail
        delegate: delegateThumbnail
        orientation: ListView.Horizontal
    }
    Component {
        id: delegateThumbnail
        Rectangle {
            width: desktop_view.width / 5
            height: 100
            color: "transparent"
            Rectangle {
                width: 82
                height: 82
                radius: 8
                color: "transparent"
                anchors.centerIn: parent
                Image {
                    id: browse_img
                    source: desktop_view.model.getIcon(index, 0)
                    anchors.fill: parent
                    fillMode: Image.Stretch
                    opacity: desktop_view.currentIndex == index ? iconsOpacity : 1
                }
                Text {
                    anchors.top: browse_img.bottom
                    anchors.topMargin: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    id: appName
                    text: qsTr(desktop_view.model.getAppName(index, 0))
                    color: "white"
                    font.pixelSize: 15
                    font.bold: true
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        desktop_view.currentIndex = index
                        mainSwipeView.currentIndex = index + 1
                        backFlag = true
                    }
                    onPressed:{ desktop_view.currentIndex = index; iconsOpacity = 0.8}
                    onEntered: {desktop_view.currentIndex = index; iconsOpacity = 0.5}
                    onReleased: iconsOpacity = 1
                    onExited: iconsOpacity = 1
                }
            }
        }
    }
    Rectangle {
        id: colckbg
        anchors.top: parent.top
        anchors.topMargin: 40 * parent.height / 480
        anchors.left: parent.left
        anchors.leftMargin: (parent.width - 190 * 3) / 4
        width: 190
        height: 270
        radius: 10
        color: "#99333333"
        Clock{
            id: clock
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10
            anchors.top: parent.top
        }

        Text {
            id: textTime
            anchors.top: clock.bottom
            anchors.topMargin: -50
            anchors.horizontalCenter: clock.horizontalCenter
            height: 25
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignHCenter
            color: "white"
            font.pixelSize: 25
            font.bold: true
            text: currentTimeString
        }

        Text {
            id: textDate
            anchors.top: textTime.bottom
            anchors.topMargin: 15
            anchors.horizontalCenter: clock.horizontalCenter
            height: 25
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignHCenter
            color: "gray"
            font.pixelSize: 15
            font.bold: true
            text: currentDateString
        }
    }

    Rectangle {
        id: musicbg
        anchors.top: parent.top
        anchors.topMargin: 40 * parent.height / 480
        anchors.left: colckbg.right
        anchors.leftMargin: (parent.width - 190 * 3) / 4
        width: 190
        height: 270
        radius: 10
        color: "#99333333"
        Image {
            id: cd
            source: "qrc:/desktop/androidstyleicons/cd.png"
            width: 150
            height: 150
            anchors.bottom: parent.verticalCenter
            anchors.bottomMargin: -45
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Button{
            id: playbtn
            anchors.top: cd.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            checkable: true
            background: Image {
                id: playImage
                source: !myMusicstate ? "qrc:/desktop/images/play.png" : "qrc:/desktop/images/pause.png"
            }
            onClicked: {
                myMusicstate = !myMusicstate
            }
        }
    }

    Rectangle {
        id: weatherbg
        anchors.top: parent.top
        anchors.topMargin: 40 * parent.height / 480
        anchors.left: musicbg.right
        anchors.leftMargin: (parent.width - 190 * 3) / 4
        width: 190
        height: 270
        radius: 10
        color: "#99333333"
        Image {
            id: weatherImage
            width: 150
            height: 150
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            mipmap: true
            fillMode: Image.PreserveAspectFit
            source: myModel.ready ? (dayOrNight >= 18 || dayOrNight <= 6 ? myModel.weatherData[0].nightPicture
                                                                         : myModel.weatherData[0].dayPicture) : ""
        }
        Text {
            id: cTemp
            anchors.top: weatherImage.bottom
            anchors.left: parent.left
            anchors.leftMargin: 50
            font.pointSize: 30
            font.family: "方正"
            color: "white"
            font.bold: true
            text:myModel.ready ? myModel.cTemp + "°" : "..."
        }
        Text {
            id: weatherType
            anchors.left: cTemp.right
            anchors.top: weatherImage.bottom
            anchors.topMargin: 15
            font.pointSize: 14
            font.family: "方正"
            color: "white"
            font.bold: true
            text: myModel.ready ? myModel.weatherData[0].weather : "..."
        }
        Text {
            id: city
            anchors.top: weatherType.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: weatherImage.horizontalCenter
            font.pointSize: 15
            font.family: "方正"
            text: myModel.ready ? myModel.city + "市" : "..."
            color: "gray"
        }
    }
}

