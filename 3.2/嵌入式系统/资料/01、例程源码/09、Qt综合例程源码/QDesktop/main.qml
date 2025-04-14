/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   QDesktop
* @brief         main.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-31
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import QtQuick.Controls 1.2
import an.weather 1.0
import an.model 1.0
import myDesktop 1.0
import "./calculator"
import "./weather"
import "./desktop"
import "./music"
import "./media"
import "./wireless"
import "./fileview"
import "./tcpclient"
import "./tcpserver"
import "./alarms"
import "./udpchat"
import "./photoview"
import "./aircondition"
import "./iotest"
import "./sensor"
import "./system"
import "./radio"
import "./settings"
import "./cameramedia"

Window {
    id: mainWindow
    property int dayOrNight: 0
    property bool myMusicstate: false
    property string currentTimeString
    property string currentTimeStringSecond
    property string currentDateString
    property string currentWeekString
    property bool rebootFlag: false
    property bool poweroffFlag: false
    property bool backFlag: false
    visible: true
    width: WINenv ? 800 : Screen.desktopAvailableWidth
    height: WINenv ? 480 : Screen.desktopAvailableHeight
    property bool smallScreen: width == 480 ? true : false

    //flags:Qt.FramelessWindowHint

    MyDesktop {
        id: myDesktop
        onSysVolumeChanged: {
            system_volume_slider.value = Number(sysVolume)
        }
        onCpuTempChanged: {
            displayCpuTemp.text = "CPU:" + cpuTemp + "℃"
        }
    }

    Timer{
        id: volume_timer
        interval: 5000
        repeat: true
        running: false
        onTriggered:{
            volume_timer.stop()
            desktop_bell.checked = false
        }
    }

    Timer{
        id: menu_timer
        interval: 5000
        repeat: true
        running: false
        onTriggered:{
            menu_timer.stop()
            if (menuBt.checked){
                hideMenu.start()
                xMoveRight.start()
                opacityAnHide.start()
                menuBt.checked = false
            }
        }
    }
    RoundButton {
        id: backBtn
        z: 109
        x: parent.x + parent.width - 90
        y: parent.y + parent.height / 2 - 50
        width: 60
        height: 60
        focus: false
        visible: mainSwipeView.currentIndex != 0
        hoverEnabled: enabled
        opacity: hovered ? 1 : 0.5
        background: Rectangle{
            color: "#55ffffff"
            radius: parent.width / 2
        }
        Image {
            anchors.centerIn: parent
            id: backImage
            source: "qrc:/desktop/images/back.png"
        }
        MouseArea {
            anchors.fill: parent
            drag.target: backBtn
            drag.minimumX: 0
            drag.minimumY: 0
            drag.maximumX: mainWindow.width - 60
            drag.maximumY: mainWindow.height - 60
            onClicked: {
                if (mainSwipeView.currentIndex == 3)
                    mainSwipeView.currentIndex = 0
                if (mainSwipeView.currentIndex != 0 && mainSwipeView.currentIndex != 3 && backFlag != true)
                    WINStyle ? mainSwipeView.currentIndex = 0 : mainSwipeView.currentIndex = 3
                if (backFlag)
                    mainSwipeView.currentIndex = 0
            }
        }
    }
    Rectangle {
        anchors.fill: parent
        color: "#1f1e58"
    }

    Item {
        id: menuItem
        z: 111
        //visible: mainSwipeView.currentIndex == 0
        visible: false
        anchors.fill: parent

        Rectangle {
            id : menuButtonsBg
            color: "transparent"
            height: 30
            width: 0
            x: menuBt.x
            y: menuBt.y
            radius: 5
            Item {
                visible: menuButtonsBg.x < menuBt.x - 50
                anchors.fill: parent
                Image {
                    id: closeImage
                    source: "qrc:/desktop/images/close.png"
                    anchors.centerIn: parent
                    antialiasing: true
                    width: 25
                    height: 25
                    MouseArea {
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            Qt.quit()
                        }
                        onEntered: closeImage.opacity = 0.5
                        onExited: closeImage.opacity = 0.8
                    }
                }
                Image {
                    id: rebootImage
                    source: "qrc:/desktop/images/reboot.png"
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: 25
                    height: 25
                    antialiasing: true
                    MouseArea {
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            rebootFlag = true
                            Qt.quit()
                        }
                        onEntered: rebootImage.opacity = 0.5
                        onExited: rebootImage.opacity = 0.8
                    }
                }

                Image {
                    id: poweroffImage
                    source: "qrc:/desktop/images/poweroff.png"
                    anchors.right: parent.right
                    width: 25
                    height: 25
                    antialiasing: true
                    MouseArea {
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            poweroffFlag = true
                            Qt.quit()
                        }
                        onEntered: poweroffImage.opacity = 0.5
                        onExited: poweroffImage.opacity = 0.8
                    }
                }
            }
        }
        PropertyAnimation{
            id: showMenu
            target: menuButtonsBg
            properties: "width"
            from: 0
            to: 180
            duration: 500
        }
        PropertyAnimation{
            id: hideMenu
            target: menuButtonsBg
            properties: "width"
            from: 180
            to: 0
            duration: 500
        }
        PropertyAnimation{
            id: xMoveLeft
            target: menuButtonsBg
            properties: "x"
            from: menuBt.x
            to: menuBt.x - 190
            duration: 500
        }

        PropertyAnimation{
            id: xMoveRight
            target: menuButtonsBg
            properties: "x"
            from: menuBt.x -190
            to: menuBt.x
            duration: 500
        }
        PropertyAnimation{
            id: opacityAnShow
            target: menuButtonsBg
            properties: "opacity"
            from: 0.0
            to: 0.8
            duration: 500
        }
        PropertyAnimation{
            id: opacityAnHide
            target: menuButtonsBg
            properties: "opacity"
            from: 0.8
            to: 0.0
            duration: 500
        }
        Button {
            id: menuBt
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.topMargin: 5
            anchors.top: parent.top
            width: 40
            height: 20
            checked: false
            style:ButtonStyle {
                background: Rectangle{
                    color: "transparent"
                }
            }
            Text {
                id: menuText
                text: qsTr("…")
                anchors.centerIn: parent
                color: menuBt.hovered ? "white" : "#ccffffff"
                font.pixelSize: 30
            }
            onClicked: {
                menu_timer.start()
                if (!checked) {
                    showMenu.start()
                    xMoveLeft.start()
                    opacityAnShow.start()
                } else {
                    hideMenu.start()
                    xMoveRight.start()
                    opacityAnHide.start()
                }
                checked = !checked
            }
        }

        Button {
            id: desktop_bell
            //visible: WINStyle ? menuBt.visible : false
            visible: false
            anchors.top: menuBt.bottom
            anchors.horizontalCenter: menuBt.horizontalCenter
            anchors.topMargin: 8
            width: 22
            height: 26
            checked: false
            style: ButtonStyle {
                background: Rectangle {
                    color: "transparent"
                    Image {
                        anchors.fill: parent
                        source: "qrc:/desktop/images/bell.png"
                        opacity: desktop_bell.hovered ? 1 : 0.8
                    }
                }
            }
            onClicked: {
                volume_timer.start()
                checked = !checked
                if (checked)
                    myDesktop.getSystemVolume()
            }
        }

        Rectangle {
            anchors.centerIn: parent
            width: 300
            height: 80
            radius: 20
            visible: desktop_bell.checked
            color: "#424242"
            Slider {
                id: system_volume_slider
                height: 50
                width: 280
                anchors.centerIn: parent
                updateValueWhileDragging: true
                stepSize: 1
                maximumValue: 127
                property bool handled: false
                onPressedChanged: {
                    handled = !handled
                    volume_timer.start()
                    if (!handled)
                        myDesktop.setSystemVolume(value)
                }
                onValueChanged: {
                    volume_timer.stop()
                }
                style: SliderStyle {
                    groove: Rectangle {
                        width: control.width
                        height: 3
                        radius: 1
                        color: "white"
                        Rectangle {
                            width: styleData.handlePosition
                            height: 3
                            color: "#27e0fb"
                            radius: 1
                        }
                    }
                    handle: Rectangle {
                        width: 1
                        height: 30
                        color: "transparent"
                    }
                }
            }
        }
    }

    Item {
        id: topMenu
        anchors.top: mainWindow.top
        anchors.left: mainWindow.left
        width: mainWindow.width
        height: 30
        Rectangle {
            anchors.fill: topMenu
            //color: "#55111111"
            color: "transparent"
            Text {
                id: netText
                text: qsTr("www.openedv.com")
                color: "white"
                font.bold: true
                font.pixelSize: 20
                anchors.left: alientek.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            Image {
                id: alientek
                source: "qrc:/desktop/images/alientek.png"
                anchors.left: parent.left
                //anchors.leftMargin: mainSwipeView.currentIndex == 0 ? 5 : 40
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: displayCpuTemp
                anchors.verticalCenter: alientek.verticalCenter
                anchors.left: netText.right
                anchors.leftMargin: 10
                text: qsTr("CPU:50℃")
                color: "white"
                font.bold: true
                font.pixelSize: 20
            }
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        color: "transparent"
        SwipeView {
            id: mainSwipeView
            anchors.fill: parent
            interactive: false
            orientation: ListView.Vertical
            Component.onCompleted: {
                contentItem.highlightMoveDuration = 0
            }
            // Desktop 与 WinStyleDesktop 只能二选一，选择DeskTop需要取消注释Applications，
            // 这里设计两款桌面，一种是类似是安卓类型，一种是类似Win10类型
            //DeskTop{}
            WinStyleDesktop{}
            Music{}
            Alarm{}
            //Media{}
            //Applications{}
            Weather{}
            Radio{}
            Calculator{}
            TcpServer{}
            TcpClient{}
            UdpChat{}
            PhotoView{}
            FileView{}
            Aircondition{}
            Iotest{}
            Sensor{}
            MyWireless{}
            System{}
            Settings{}
            MyCameraMedia{}
            onCurrentIndexChanged : {
                mainSwipeView.currentItem.visible = true
                for (var i = 0; i < mainSwipeView.count; i++){
                    if (i != mainSwipeView.currentIndex)
                        mainSwipeView.itemAt(i).visible = false
                }
            }
        }
    }


    Text {
        id: mainTimeText
        visible: false // mainSwipeView.currentIndex == 0
        text: currentTimeString
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 50
        color: "white"
        font.pixelSize: 30
        font.bold: true
    }
    Text {
        id: weekText
        visible: false // mainSwipeView.currentIndex == 0
        text: currentWeekString
        anchors.top: mainTimeText.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: mainTimeText.horizontalCenter
        color: "#99eeeeee"
        font.pixelSize: 15
        font.bold: true
    }

    function currentDate(){
        return Qt.formatDateTime(new Date(), "ddd yyyy年MM月dd日" )
    }

    function currentWeek(){
        var dayofWeek = ""
        var str = Qt.formatDateTime(new Date(), "ddd" )
        switch (str){
        case "Sun":
        case "周日":
            dayofWeek = "周日"
            break;
        case "Mon":
        case "周一":
            dayofWeek = "周一"
            break;
        case "Tue":
        case "周二":
            dayofWeek = "周二"
            break;
        case "Wed":
        case "周三":
            dayofWeek = "周三"
            break;
        case "Thu":
        case "周四":
            dayofWeek = "周四"
            break;
        case "Fri":
        case "周五":
            dayofWeek = "周五"
            break;
        case "Sat":
        case "周六":
            dayofWeek = "周六"
            break;
        }
        return dayofWeek + Qt.formatDateTime(new Date(), ",MM月dd日" )
    }

    function currentTime(){
        dayOrNight = new Date().getHours()
        return Qt.formatDateTime(new Date(), "hh:mm" )
    }

    function currentTimeSecond(){
        dayOrNight = new Date().getHours()
        return Qt.formatDateTime(new Date(), "hh:mm:ss" )
    }

    Timer{
        id: timer
        interval: 1000
        repeat: true
        running: true
        property int welcomeTimerCount: 0
        onTriggered:{
            currentTimeString= currentTime()
            currentDateString = currentDate()
            currentWeekString = currentWeek()
            currentTimeStringSecond = currentTimeSecond()
            if (welcomeTimerCount < 4)
                welcomeTimerCount++
            if (welcomeTimerCount == 2)
                welcome_text.text = "欢迎"
            if (welcomeTimerCount == 4){
                myDesktop.restoreMixerSettings()
                welcome_display.visible = false
                welcomeTimerCount++
            }
        }
        Component.onCompleted: {
            currentTimeString= currentTime()
            currentDateString = currentDate()
            currentWeekString = currentWeek()
        }
    }
    Item {
        id: welcome_display
        anchors.fill: parent
        //visible: !WINenv
        z: 120
        Flickable {
            anchors.fill: parent
            contentWidth: parent.width
            contentHeight: parent.height +20
            Rectangle {
                anchors.fill: parent
                color: "#1f1e58"
                Text {
                    id: welcome_text
                    text: qsTr("正在初始化，请稍候...")
                    color: "white"
                    font.pixelSize: 30
                    font.bold: true
                    anchors.centerIn: parent
                }
            }
        }
    }
}
