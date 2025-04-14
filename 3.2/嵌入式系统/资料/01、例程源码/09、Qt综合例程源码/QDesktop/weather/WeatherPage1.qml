/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   weather
* @brief         Weatherwpage11.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-31
*******************************************************************/
import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Window 2.3
import QtCharts 2.2
import an.weather 1.0
import an.model 1.0

Item {
    id: w1

    Connections{
        target: myModel
        onReadyChanged:{
            if (myModel.ready)
                refreshAnimation.restart();
        }
    }

    NumberAnimation {
        id: refreshAnimation
        target: wpage1
        property: "y"
        duration: 200
        from: wpage1.pullHeight
        to: 0
        easing.type: Easing.Linear
    }

    Flickable {
        id: wpage1
        anchors.fill: parent
        interactive: true
        contentWidth: smallScreen ? parent.width + 320 : parent.width
        contentHeight: smallScreen ? parent.height + 208 : parent.height + 1
        property int pullHeight: 20

        states: [
            State {
                id: downRefreshState
                name: "downRefresh"
                when: wpage1.contentY < -wpage1.pullHeight
                StateChangeScript{
                    script:{
                        print("下拉刷新")
                        wpage1.y = wpage1.pullHeight
                        myModel.downRefresh();
                    }
                }
            },
            State {
                when: wpage1.contentX > 400
                StateChangeScript{
                    script:{
                        weatherSwipeView.setCurrentIndex(1)
                        print("进入页面二")
                        WeatherPage1.visible = false
                        WeatherPage2.visible = true

                    }
                }
            }
        ]
        ScrollBar.vertical: ScrollBar {
            visible: smallScreen ? true : false
            id: scrollBarVertical1
            z: 20
            width: 5
            hoverEnabled: true
            background: Rectangle {color: "#33bbbbbb"}
            onActiveChanged: {
                active = true;
            }
            Component.onCompleted: {
                scrollBarVertical1.active = true;
            }
            contentItem: Rectangle{
                implicitWidth: 5
                implicitHeight: 100
                radius: 2
                color: scrollBarVertical1.hovered ? "#25cfea" : "#88bbbbbb"
            }
        }
        ScrollBar.horizontal: ScrollBar {
            visible: smallScreen ? true : false
            id: scrollBarHorizontal1
            z: 20
            height: 5
            hoverEnabled: true
            background: Rectangle {color: "#33bbbbbb"}
            onActiveChanged: {
                active = true;
            }
            Component.onCompleted: {
                scrollBarHorizontal1.active = true;
            }
            contentItem: Rectangle{
                implicitWidth: 100
                implicitHeight: 5
                radius: 2
                color: scrollBarHorizontal1.hovered ? "#25cfea" : "#88bbbbbb"
            }
        }

        Item {
            id: content
            anchors.fill: parent

            Image {
                id: location
                width: 30
                height: 30
                source: "qrc:/weather/image/location.png"
                anchors.right: city.left
                anchors.rightMargin: 15
                anchors.bottom: city.bottom
                anchors.bottomMargin: -5
            }
            Text {
                id: city
                anchors.top: parent.top
                anchors.topMargin: parent.height/3
                anchors.left: cTemp.left
                anchors.leftMargin: 30
                font.pointSize: 25
                font.family: "方正"
                text: myModel.ready ? myModel.city : "载入中..."
                color: "white"
            }
            Text {
                id: updateMsg
                anchors.horizontalCenter: city.horizontalCenter
                anchors.top: city.bottom
                anchors.topMargin: 5
                font.pointSize: 8
                font.family: "方正"
                color: "#EEEEEE"
                text: myModel.ready ? "天气数据已经更新" : "天气数据更新中...."
            }
            Text {
                id: temperature
                visible: false
                anchors.horizontalCenter: updateMsg.horizontalCenter
                anchors.top: cTemp.bottom
                anchors.topMargin: 10
                font.pointSize: 14
                font.family: "方正"
                text: myModel.ready ? myModel.weatherData[0].minTemperature
                                      + " ~ " + myModel.weatherData[0].maxTemperature + "℃": "载入中..."
                color: "white"
            }
            Text {
                id: cTemp
                anchors.bottom: city.top
                anchors.left:  parent.horizontalCenter
                anchors.leftMargin: 160
                font.pointSize: 80
                font.family: "方正"
                color: "white"
                text:myModel.ready ? myModel.cTemp + "°" : "..."
            }
            Rectangle {
                anchors.bottom: city.top
                anchors.bottomMargin: 5
                anchors.left: location.left
                anchors.right: city.right
                height: 1
                color: "#80eeeeee"
            }
            Text {
                id: weatherType
                anchors.top: weatherImage.bottom
                anchors.horizontalCenter: weatherImage.horizontalCenter
                anchors.leftMargin: 20
                font.pointSize: 14
                font.family: "方正"
                color: "white"
                text: myModel.ready ? myModel.weatherData[0].weather : "载入中..."
            }
            Image {
                id: weatherImage
                width: 150
                height: 150
                anchors.top: cTemp.bottom
                anchors.topMargin: -50
                anchors.right: parent.horizontalCenter
                anchors.rightMargin: 200
                mipmap: true
                fillMode: Image.PreserveAspectFit
                source: myModel.ready ? (dayOrNight >= 18 || dayOrNight <= 6 ? myModel.weatherData[0].nightPicture
                                                                             : myModel.weatherData[0].dayPicture) : ""
            }
            Rectangle {
                id: tipsRect
                width:parent.width
                clip: true
                anchors.top: weatherType.bottom
                anchors.topMargin: -30
                height: 80
                color: "transparent"

                Text {
                    id: tipsText
                    visible: false
                    text: "温馨提示:"
                    anchors.left: tipsRect.left
                    anchors.leftMargin: 15
                    anchors.top: tipsRect.top
                    anchors.topMargin: 15
                    font.pointSize: 13
                    font.bold: true
                    font.family: "方正"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "#FC993C"
                }
                Flickable {
                    id: flickable2
                    anchors.left: tipsRect.left
                    anchors.leftMargin: 10
                    anchors.right: tipsRect.right
                    anchors.rightMargin: 10
                    anchors.top: tipsText.bottom
                    anchors.topMargin: 10
                    width: parent.width
                    height: parent.height
                    contentWidth: tipsView.contentWidth
                    contentHeight: tipsView.contentHeight
                    ListView {
                        id: tipsView
                        height: parent.width
                        width: parent.width
                        orientation: ListView.Horizontal
                        model: myModel.indexData
                        spacing: 20
                        delegate: Component {
                            Column {
                                Text {
                                    id: descript
                                    width: flickable2.width
                                    text: modelData.descript
                                    wrapMode: Text.Wrap
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    font.pointSize: 11
                                    font.family: "方正"
                                    color: "white"
                                }
                            }
                        }
                    }
                }
            }
            Component {
                id: delegate
                Rectangle {
                    width: weatherDatabg.width/5 - 2
                    height: weatherDatabg.height - 2
                    color: "#33bbbbbb"
                    radius: 10
                    Text{
                        id: dateText
                        width: weatherDatabg.width/5
                        height: 20
                        font.family: "方正"
                        font.pointSize: 10
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: index != 0 ? date : "今日"
                        color: "white"
                    }
                    Text {
                        id: weatherText
                        width: weatherDatabg.width/5
                        height: 20
                        font.family: "方正"
                        font.pointSize: 10
                        anchors.top: dateText.bottom
                        anchors.topMargin: 5
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: myModel.ready ? weather : "未知"
                        color: "white"
                    }
                    Image {
                        id: dayAndNight
                        anchors.top: weatherText.bottom
                        anchors.topMargin: 2
                        width: weatherDatabg.width/5
                        height: 50
                        mipmap: true
                        fillMode: Image.PreserveAspectFit
                        source: myModel.ready ? (dayOrNight >= 18 || dayOrNight <= 6? nightPicture : dayPicture) : ""
                    }

                    Text {
                        id: temperatureText
                        width: weatherDatabg.width/5
                        height: 20
                        font.family: "方正"
                        font.pointSize: 10
                        anchors.top: dayAndNight.bottom
                        anchors.topMargin: 5
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: myModel.ready ? myModel.weatherData[index].minTemperature + " ~ " +
                                              myModel.weatherData[index].maxTemperature + "℃" : "未知"
                        color: "white"
                    }

                    Text {
                        id: windText
                        width: weatherDatabg.width/5
                        height: 20
                        font.family: "方正"
                        font.pointSize: 10
                        anchors.top: temperatureText.bottom
                        anchors.topMargin: 5
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: myModel.ready ? wind : "未知"
                        color: "white"
                    }
                }
            }

            Rectangle{
                id: weatherDatabg
                anchors.top: tipsRect.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                color: "transparent"
                ListView {
                    id: futureWeather
                    width: contentWidth
                    orientation: ListView.Horizontal
                    anchors.fill: weatherDatabg
                    spacing: 2
                    model: myModel.weatherData
                    delegate: delegate
                }
            }
        }
    }
}
