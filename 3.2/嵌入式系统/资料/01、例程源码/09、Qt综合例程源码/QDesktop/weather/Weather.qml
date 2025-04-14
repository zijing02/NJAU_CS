/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   weather
* @brief         Weather.qml
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
import "../helpbutton"

Item {
    id: weather
    visible: false

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/weather.png"
        appName: "天气"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件为天气软件，可从网上获取天气！注意开发板需要联网！联网后需要在天气页面手动下拉刷新获取天气信息！第一屏获取近5天的天气数据，程序1小时自动更新一次天气数据，第二屏则是最高温与最低温的折线图，左右滑动可切换一二屏。"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }
    Component.onCompleted: {
        weather.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: weather
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
        anchors.fill: parent
        color: "transparent"
        SwipeView {
            id: weatherSwipeView
            interactive: smallScreen ? false : true
            anchors.fill: parent
            //currentIndex: tabBar.currentIndex
            WeatherPage1 {}
            WeatherPage2 {}
        }
    }
}
