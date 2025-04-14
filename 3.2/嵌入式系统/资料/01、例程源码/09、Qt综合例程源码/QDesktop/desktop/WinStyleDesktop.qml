import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick 2.9
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import QtGraphicalEffects 1.0
import QtQuick 2.4
import an.weather 1.0
import an.model 1.0
import "../settings"
Item {
    id: winDesktop
    property real albumImageOpacity: 1.0
    property string albumPath: myPhoto.currentIndex != -1 ? myPhoto.getcurrentPath() : ""
    property string weatherIconStr: "qrc:/desktop/winstyleicons/weather_app_rain.png"
    property int count: 0
    visible: true

    function checkWeatherType(arg) {
        var str = arg
        if(str.indexOf("fine") !== -1) {
            weatherIconStr = "qrc:/desktop/winstyleicons/weather_app_fine.png"
            return "qrc:/desktop/winstyleicons/weather_app_fine.png"
        }else if(str.indexOf("rain") !== -1) {
            weatherIconStr = "qrc:/desktop/winstyleicons/weather_app_rain.png"
            return "qrc:/desktop/winstyleicons/weather_app_rain.png"
        }else if(str.indexOf("snow") !== -1) {
            weatherIconStr = "qrc:/desktop/winstyleicons/weather_app_snow.png"
            return "qrc:/desktop/winstyleicons/weather_app_snow.png"
        }else if(str.indexOf("cloudy") !== -1) {
            weatherIconStr = "qrc:/desktop/winstyleicons/weather_app_cloudy.png"
            return "qrc:/desktop/winstyleicons/weather_app_cloudy.png"
        }else
            return "qrc:/desktop/winstyleicons/weather_app_rain.png"
    }

    onVisibleChanged: {
        if (winDesktop.visible)
            albumTimer.start()
        else
            albumTimer.stop()
    }

    Timer{
        id: albumTimer
        interval: 100
        running: winDesktop.visible
        repeat: true
        onTriggered:{
            count += 1
            if (count >= 150 && count < 160) {
                albumImageOpacity -= 0.1
                if (albumImageOpacity <= 0)
                    albumImageOpacity = 0
            }
            if (count == 160 && myPhoto.currentIndex != -1) {
                albumImageOpacity = 0
                myPhoto.setCurrentIndex(myPhoto.currentIndex + 1)
            }
            if (count >= 160 && myPhoto.currentIndex != -1) {
                albumPath = myPhoto.getcurrentPath()
                albumImageOpacity = albumImageOpacity + 0.1
                if (albumImageOpacity >= 1.0)
                    albumImageOpacity = 1.0
            }
            if (count >= 170) {
                count = 0
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
        interactive: smallScreen ? false : true
        Item {
            id: page1
            Flickable {
                id: first
                anchors.fill: parent
                interactive: smallScreen ? true : false
                contentWidth: smallScreen ? parent.width + 320 : parent.width
                contentHeight: smallScreen ? parent.height + 208 : parent.height
                states: [
                    State {
                        when: first.contentX > 400
                        StateChangeScript{
                            script:{
                                swipeView_app.setCurrentIndex(1)
//                                print("进入页面二")
                                page1.visible = false
                                page2.visible = true

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
                Text {
                    id: tips1
                    text: qsTr("生活动态")
                    color: "white"
                    font.pixelSize: 17
                    font.bold: true
                    anchors.bottom: timeRect.top
                    anchors.bottomMargin: 16
                    anchors.left: timeRect.left
                }
                Text {
                    id: tips2
                    text: qsTr("播放和浏览")
                    color: "white"
                    font.pixelSize: 17
                    font.bold: true
                    anchors.bottom: timeRect.top
                    anchors.bottomMargin: 16
                    anchors.left: calc_app.left
                }
                Text {
                    id: tips3
                    text: qsTr("测试与应用")
                    color: "white"
                    font.pixelSize: 17
                    font.bold: true
                    anchors.bottom: timeRect.top
                    anchors.bottomMargin: 16
                    anchors.left: aircondition_app.left
                }
                Item {
                    id: timeRect
                    anchors.top: calc_app.top
                    anchors.right: calc_app.left
                    anchors.rightMargin: 25
                    width: 200
                    height: 102
                    MouseArea {
                        anchors.fill : parent
                        onClicked: {
                            mainSwipeView.setCurrentIndex(2)
                        }
                    }
                    Image {
                        anchors.fill: parent
                        source: "qrc:/desktop/images/bgTime.png"
                    }

                    Text {
                        id: textTime
                        anchors.top: parent.top
                        anchors.topMargin: 15
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignHCenter
                        color: "white"
                        font.pixelSize: 50
                        font.bold: true
                        text: currentTimeString
                    }

                    Text {
                        id: textDate
                        anchors.top: textTime.bottom
                        anchors.topMargin: 13
                        anchors.left: textTime.left
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignHCenter
                        color: "#bbffffff"
                        font.pixelSize: 15
                        font.bold: true
                        text: currentDateString
                    }
                }

                Item {
                    id: weatherRect
                    anchors.top: timeRect.bottom
                    anchors.left: timeRect.left
                    anchors.topMargin: 5
                    width: 200
                    height: 100
                    MouseArea {
                        anchors.fill : parent
                        onClicked: {
                            mainSwipeView.setCurrentIndex(3)
                        }
                    }
                    Image {
                        anchors.fill: parent
                        source: "qrc:/desktop/images/bgWeather.png"
                    }
                    Image {
                        id: weatherImage
                        width: 80
                        height: 80
                        anchors.left: parent.left
                        anchors.top: parent.top
                        mipmap: true
                        fillMode: Image.PreserveAspectFit
                        source: myModel.ready ? (dayOrNight >= 18 || dayOrNight <= 6 ? myModel.weatherData[0].nightPicture
                                                                                     : myModel.weatherData[0].dayPicture) : ""
                    }
                    Text {
                        id: cTemp
                        anchors.top: parent.top
                        anchors.right: parent.right
                        font.pixelSize: 40
                        font.family: "方正"
                        color: "white"
                        font.bold: true
                        text:myModel.ready ? myModel.cTemp + "°" : "..."
                    }
                    Text {
                        id: weatherType
                        anchors.left: cTemp.left
                        anchors.top: cTemp.bottom
                        anchors.topMargin: 10
                        anchors.horizontalCenter: cTemp.horizontalCenter
                        font.pixelSize: 15
                        font.family: "方正"
                        color: "white"
                        font.bold: true
                        text: myModel.ready ? myModel.weatherData[0].weather : "..."
                    }
                    Text {
                        id: city
                        anchors.top: weatherType.bottom
                        anchors.topMargin: 10
                        anchors.horizontalCenter: cTemp.horizontalCenter
                        anchors.left: weatherType.left
                        font.pixelSize: 15
                        font.bold: true
                        font.family: "方正"
                        text: myModel.ready ? myModel.city + "市" : "..."
                        color: "white"
                    }
                }

                Item {
                    id: albumRect
                    anchors.top: weatherRect.bottom
                    anchors.left: weatherRect.left
                    anchors.topMargin: 7
                    width: 200
                    height: 102
                    Image {
                        anchors.fill: parent
                        opacity: albumImageOpacity
                        source: albumPath
                    }
                    Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                        border.width: 5
                        border.color: "#bbbbbbbb"
                    }
                    MouseArea {
                        anchors.fill : parent
                        onClicked: {
                            mainSwipeView.setCurrentIndex(9)
                        }
                    }
                }

                Button {
                    id: calc_app
                    anchors.right: parent.horizontalCenter
                    anchors.rightMargin: 50
                    anchors.bottom: parent.verticalCenter
                    anchors.bottomMargin: 50
                    width: 100
                    height: 102
                    onClicked: {
                        mainSwipeView.setCurrentIndex(5)
                    }
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/calc_app.png"
                    }
                    Text {
                        text: qsTr("计算器")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.leftMargin: 3
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#0072c5"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }
                }

                Button {
                    id: meida_app
                    anchors.left: calc_app.right
                    anchors.leftMargin: 5
                    anchors.top: calc_app.top
                    width: 100
                    height: 102
                    onClicked: {
                        mainSwipeView.setCurrentIndex(2)
                    }
                    Image {
                        width: 100
                        height: 102
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/alarm_app.png"
                    }
                    Text {
                        text: qsTr("闹钟")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.left: parent.left
                        anchors.leftMargin: 3
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#0072c5"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }
                }

                Button {
                    id: radio_app
                    anchors.left: meida_app.right
                    anchors.leftMargin: 5
                    anchors.top: calc_app.top
                    width: 100
                    height: 102
                    onClicked: {
                        mainSwipeView.setCurrentIndex(4)
                    }
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/radio_app.png"
                    }
                    Text {
                        text: qsTr("Radio")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.leftMargin: 3
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#0378d8"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }
                }

                Button {
                    id: aircondition_app
                    anchors.left: radio_app.right
                    anchors.leftMargin: 25
                    anchors.top: calc_app.top
                    width: 100
                    height: 102
                    onClicked: {
                        mainSwipeView.setCurrentIndex(11)
                    }
                    Text {
                        text: qsTr("空调")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.leftMargin: 3
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#ca2c2b"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/aircondition_app.png"
                    }
                }

                Button {
                    id: weather_app
                    anchors.left: aircondition_app.left
                    anchors.top: aircondition_app.bottom
                    anchors.topMargin: 5
                    width: 205
                    height: 100
                    Image {
                        id: weather_icon
                        width: 205
                        height: 100
                        anchors.centerIn: parent
                        source: myModel.ready ? checkWeatherType(myModel.weatherData[0].nightPicture) : weatherIconStr
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            weather_icon.width = 207
                            weather_icon.height = 102
                        }
                        onExited: {
                            weather_icon.width = 205
                            weather_icon.height = 100
                        }
                        onClicked: {
                            mainSwipeView.setCurrentIndex(3)
                        }
                    }

                    Text {
                        text: qsTr("天气")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.leftMargin: 3
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                }
                Button {
                    id: settings_app
                    anchors.left: weather_app.left
                    anchors.top: weather_app.bottom
                    anchors.topMargin: 5
                    width: 100
                    height: 102
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#80397b"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/settings_app.png"
                    }
                    onClicked: {
                        mainSwipeView.setCurrentIndex(16)
                    }
                    Text {
                        text: qsTr("设置")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.leftMargin: 3
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                }


                Button {
                    id: key_app
                    anchors.left: aircondition_app.right
                    anchors.leftMargin: 5
                    anchors.top: calc_app.top
                    width: 100
                    height: 102
                    onClicked: {
                        mainSwipeView.setCurrentIndex(12)
                    }
                    Text {
                        text: qsTr("按键")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.leftMargin: 3
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#d0282f"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/iotest_app.png"
                    }
                }

                Button {
                    id: album_app
                    anchors.top: calc_app.bottom
                    anchors.topMargin: 5
                    anchors.left: calc_app.left
                    width: 100
                    height: 102
                    Image {
                        id: album_icon
                        width: 100
                        height: 102
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/album_app.png"
                    }
                    Text {
                        text: qsTr("相簿")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.leftMargin: 3
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "transparent"
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            album_icon.width = 102
                            album_icon.height = 104
                        }
                        onExited: {
                            album_icon.width = 100
                            album_icon.height = 102
                        }
                        onClicked: {
                            mainSwipeView.setCurrentIndex(9)
                        }
                    }
                }

                Button {
                    id: music_app
                    anchors.top: calc_app.bottom
                    anchors.topMargin: 5
                    anchors.left: album_app.right
                    anchors.leftMargin: 5
                    width: 205
                    height: 102
                    Image {
                        id: music_icon
                        width: 205
                        height: 102
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/music_app_icon.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            music_icon.width = 207
                            music_icon.height = 104
                        }
                        onExited: {
                            music_icon.width = 205
                            music_icon.height = 102
                        }
                        onClicked: {
                            mainSwipeView.setCurrentIndex(1)
                        }
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "transparent"
                        }
                    }
                }

                Button {
                    id: tcpServer_app
                    anchors.top: music_app.bottom
                    anchors.topMargin: 5
                    anchors.left: album_app.left
                    width: 100
                    height: 102
                    onClicked: {
                        mainSwipeView.setCurrentIndex(6)
                    }
                    Text {
                        text: qsTr("Tcp-S")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.left: parent.left
                        anchors.leftMargin: 3
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#80397b"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/tcp_s.png"
                    }
                }

                Button {
                    id: tcpClient_app
                    anchors.top: music_app.bottom
                    anchors.topMargin: 5
                    anchors.left: tcpServer_app.right
                    anchors.leftMargin: 5
                    width: 100
                    height: 102
                    onClicked: {
                        mainSwipeView.setCurrentIndex(7)
                    }
                    Text {
                        text: qsTr("Tcp-C")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.left: parent.left
                        anchors.leftMargin: 3
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#de3b00"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/tcp_c.png"
                    }
                }

                Button {
                    id: file_app
                    anchors.top: music_app.bottom
                    anchors.topMargin: 5
                    anchors.left: tcpClient_app.right
                    anchors.leftMargin: 5
                    width: 100
                    height: 102
                    onClicked: {
                        mainSwipeView.setCurrentIndex(10)
                    }
                    Text {
                        text: qsTr("文件")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.left: parent.left
                        anchors.leftMargin: 3
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#cd2930"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }

                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/file_app.png"
                    }
                }
            }
        }
        Item {
            id:page2
            visible: smallScreen ? false : true
            Flickable {
                id: second
                anchors.fill: parent
                contentWidth: smallScreen ? parent.width + 320 : parent.width
                contentHeight: smallScreen ?  parent.height + 208 : parent.height
                states: [
                    State {
                        when: second.contentX < -40
                        StateChangeScript{
                            script:{
                                swipeView_app.setCurrentIndex(0)
                                //print("返回页面一")
                                page2.visible = false
                                page1.visible = true
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
                Text {
                    id: tips4
                    text: qsTr("无线与社交")
                    color: "white"
                    font.pixelSize: 17
                    font.bold: true
                    anchors.bottom: udpChat_app.top
                    anchors.bottomMargin: 16
                    anchors.left: udpChat_app.left
                }

                Text {
                    id: tips5
                    text: qsTr("系统/媒体")
                    color: "white"
                    font.pixelSize: 17
                    font.bold: true
                    anchors.bottom: udpChat_app.top
                    anchors.bottomMargin: 16
                    anchors.left: system_app.left
                }

                Button {
                    id: udpChat_app
                    anchors.right: parent.horizontalCenter
                    anchors.rightMargin: 285
                    anchors.bottom: parent.verticalCenter
                    anchors.bottomMargin: 50
                    width: 100
                    height: 102
                    onClicked: {
                        mainSwipeView.setCurrentIndex(8)
                    }
                    Text {
                        text: qsTr("UDP-CHAT")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.left: parent.left
                        anchors.leftMargin: 3
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#4a419c"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/udpchat_app.png"
                    }
                }

                Button {
                    id: system_app
                    anchors.left: wifi_app.right
                    anchors.leftMargin: 25
                    anchors.top: wifi_app.top
                    width: 100
                    height: 102
                    onClicked: {
                        mainSwipeView.setCurrentIndex(15)
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#f6520a"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }
                    Image {
                        width: 100
                        height: 102
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/system_app.png"
                    }
                    Text {
                        text: qsTr("系统")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.leftMargin: 3
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                }
                Button {
                    id: camera_app
                    anchors.left: system_app.right
                    anchors.top: system_app.top
                    anchors.leftMargin: 5
                    width: 205
                    height: 102
                    onClicked: {
                        mainSwipeView.setCurrentIndex(17)
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#d0282f"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/camera_app.png"
                    }
                    Text {
                        text: qsTr("摄像头/视频")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.leftMargin: 3
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                }

                Button {
                    id: wifi_app
                    anchors.left: udpChat_app.right
                    anchors.leftMargin: 5
                    anchors.top: udpChat_app.top
                    width: 100
                    height: 102
                    onClicked: {
                        mainSwipeView.setCurrentIndex(14)
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#108840"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }
                    Image {
                        width: 100
                        height: 102
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/wirless_app.png"
                    }
                    Text {
                        text: qsTr("WIFI")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.leftMargin: 3
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                }

                Button {
                    id: sensor_app
                    anchors.left: udpChat_app.left
                    anchors.top: udpChat_app.bottom
                    anchors.topMargin: 5
                    width: 205
                    height: 102
                    onClicked: {
                        mainSwipeView.setCurrentIndex(13)
                    }
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#5dacee"
                            border.color: "#bbbbbbbb"
                            border.width: control.hovered ? 1 : 0
                        }
                    }
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/desktop/winstyleicons/sensor_app.png"
                    }
                    Text {
                        text: qsTr("传感器")
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                        anchors.leftMargin: 3
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }
                }
            }
        }
    }
}
