/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   weather
* @brief         WeatherPage2.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-31
*******************************************************************/
import QtQuick 2.7
import QtQuick.Controls 2.2
import QtCharts 2.2
import an.weather 1.0
import an.model 1.0
Item{
    id: w2

    Connections{
        target: myModel
        onReadyChanged:{
            if (myModel.ready){
                refreshAnimation.restart();
                maxSeries.clear()
                minSeries.clear()
                for (var i = 0; i <= 4; i++){
                    maxSeries.append(i + 1, myModel.weatherData[i].maxTemperature)
                    minSeries.append(i + 1, myModel.weatherData[i].minTemperature)
                }
            }
        }
    }

    NumberAnimation{
        id: refreshAnimation
        target: wpage2
        property: "y"
        duration: 200
        from: wpage2.pullHeight
        to: 0
        easing.type: Easing.Linear
    }
    Rectangle {
        anchors.fill: parent
        color: "transparent"

        Flickable {
            id: wpage2
            anchors.fill: parent
            interactive: true
            contentWidth: smallScreen ? parent.width + 320 : parent.width
            contentHeight: smallScreen ? parent.height + 208 : parent.height + 1
            property int pullHeight: 20

            states: [
                State {
                    id: downRefreshState
                    name: "downRefresh"
                    when: wpage2.contentY < -wpage2.pullHeight
                    StateChangeScript{
                        script:{
                            print("下拉刷新")
                            wpage2.y = wpage2.pullHeight
                            myModel.downRefresh();
                        }
                    }
                },
                State {
                    when: wpage2.contentX < -40
                    StateChangeScript{
                        script:{
                            weatherSwipeView.setCurrentIndex(0)
                            print("返回页面一")
                            WeatherPage1.visible = true
                            WeatherPage2.visible = false
                        }
                    }
                }
            ]
            ScrollBar.vertical: ScrollBar {
                visible: smallScreen ? true : false
                id: scrollBarVertical2
                z: 20
                width: 5
                hoverEnabled: true
                background: Rectangle {color: "#33bbbbbb"}
                onActiveChanged: {
                    active = true;
                }
                Component.onCompleted: {
                    scrollBarVertical2.active = true;
                }
                contentItem: Rectangle{
                    implicitWidth: 5
                    implicitHeight: 100
                    radius: 2
                    color: scrollBarVertical2.hovered ? "#25cfea" : "#88bbbbbb"
                }
            }
            ScrollBar.horizontal: ScrollBar {
                visible: smallScreen ? true : false
                id: scrollBarHorizontal2
                z: 20
                height: 5
                hoverEnabled: true
                background: Rectangle {color: "#33bbbbbb"}
                onActiveChanged: {
                    active = true;
                }
                Component.onCompleted: {
                    scrollBarHorizontal2.active = true;
                }
                contentItem: Rectangle{
                    implicitWidth: 100
                    implicitHeight: 5
                    radius: 2
                    color: scrollBarHorizontal2.hovered ? "#25cfea" : "#88bbbbbb"
                }
            }

            Item {
                id: myContent
                anchors.fill: parent

                ChartView{
                    id: futureChart
                    title:qsTr("未来温度走势")
                    titleColor: "white"
                    // Set the header to be invisible
                    legend.visible:false
                    width: parent.width
                    height: parent.height
                    // anchors.fill: parent
                    anchors.centerIn: parent
                    antialiasing: true
                    backgroundColor: "transparent"
                    plotAreaColor: "transparent"
                    Component.onCompleted:{
                        futureChart.axisX().visible = false
                        futureChart.axisY().visible = false
                    }
                    ValueAxis{
                        id: axisX
                        min: 0
                        max: 6
                    }
                    ValueAxis{
                        id: axisY
                        min: -20
                        max: 80
                    }

                    LineSeries {
                        id: maxSeries
                        //name: "最高温"
                        pointLabelsFont.family: "方正"
                        pointLabelsFont.pointSize: 15
                        pointLabelsVisible: true
                        pointLabelsFormat: "@yPoint °"  //更改label的格式
                        pointLabelsColor: "white"
                        color: "white"
                        width: 2
                        axisX: axisX
                        axisY: axisY
                    }

                    LineSeries{
                        id: minSeries
                        //name: "最低温"
                        pointLabelsFont.family: "方正"
                        pointLabelsFont.pointSize: 15
                        pointLabelsVisible: true
                        pointLabelsFormat: "@yPoint °"
                        pointLabelsColor: "white"
                        color: "#8038a4d6"
                        width: 2
                        axisX: axisX
                        axisY: axisY
                    }
                }
            }
        }
    }
}
