/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   radio
* @brief         Radio.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-08-19
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.4
import radio 1.0
import "../helpbutton"
Item {
    id: myRadio
    visible: false
    property real iconsOpacity: 1.0
    property bool muteFlag: true

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/radio.png"
        appName: "收音机FM"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件为收音机App,需要额外购买tea5767收音机模块，可在淘宝自行搜索购买，接开发板5V电源和GND,SDA和SDL分别接正点原子ALPHA/MINI的拓展IO I2C2_SCL和I2C2_SDL处。
2.请将收音机模块至于室外空旷之处才能收音！程序仅供学习与参考！"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }
    Component.onCompleted: {
        myRadio.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: myRadio
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

    MyRadio {
        id: radio
        onLevelChanged: {
            levelText.text = "信号等级为:  " + readLevel
        }
    }
    Flickable {
        anchors.fill: parent
        interactive: smallScreen ? true : false
        contentWidth: smallScreen ? parent.width + 320 : parent.width
        contentHeight: smallScreen ? parent.height + 208 : parent.height

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

        Timer{
            id: timer
            interval: 100
            repeat: true
            onTriggered:{
                if (pre_button.checked)
                    slider.value -= 0.003125

                if (next_button.checked)
                    slider.value += 0.003125
            }
        }

        Button {
            id: pre_button
            anchors.top: parent.verticalCenter
            anchors.topMargin: 20
            anchors.right: parent.horizontalCenter
            anchors.rightMargin: 150
            width: 40
            height: 40
            style: ButtonStyle {
                background:Rectangle{
                    anchors.fill: parent
                    color: "transparent"
                    Image {
                        id: preView
                        source: "qrc:/radio/images/preview.png"
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                //hoverEnabled: true

                onClicked:  {
                    //pre_button.opacity = 0.5
                    slider.value -= 0.003125
                }
                //onEntered: pre_button.opacity = 0.8
                onPressed: pre_button.opacity = 0.8
                onReleased: {
                    pre_button.opacity = 1
                    pre_button.checked = false
                    timer.stop()
                }
                //onExited: pre_button.opacity = 1
                onPressAndHold: {
                    pre_button.checked = true
                    timer.start()
                }
            }
        }

        Button {
            id: next_button
            anchors.top: parent.verticalCenter
            anchors.topMargin: 20
            anchors.left: parent.horizontalCenter
            anchors.leftMargin: 150
            width: 40
            height: 40
            checkable: true
            style: ButtonStyle {
                background:Rectangle{
                    anchors.fill: parent
                    color: "transparent"
                    Image {
                        id: next
                        source: "qrc:/radio/images/next.png"
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                //hoverEnabled: true
                onClicked:  {
                    //next_button.opacity = 0.5
                    slider.value += 0.003125
                }
                //onEntered: next_button.opacity = 0.8
                onPressed: next_button.opacity = 0.8
                onReleased: {
                    next_button.opacity = 1
                    next_button.checked = false
                    timer.stop()
                }
                //onExited: next_button.opacity = 1
                onPressAndHold: {
                    next_button.checked = true
                    timer.start()
                }
            }
        }

        Text {
            id: levelText
            text: qsTr("信号等级为: 0")
            anchors.top: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            font.bold: true
            font.pixelSize: 20
            color: "white"
        }

        Text {
            id: fm
            text: qsTr("FM")
            color: "white"
            anchors.right: fmValue.left
            anchors.rightMargin: 30
            anchors.bottom: parent.verticalCenter
            anchors.bottomMargin: 20
            font.pixelSize: 30
        }

        Text {
            id: fmUnit
            text: qsTr("MHz")
            color: "white"
            anchors.left: fmValue.right
            anchors.leftMargin: 30
            anchors.bottom: parent.verticalCenter
            anchors.bottomMargin: 20
            font.pixelSize: 30
        }

        Text {
            id: mute
            text: !muteFlag ? "Mute" : "UnMute"
            color: "white"
            anchors.left: next_button.right
            anchors.leftMargin: 50
            anchors.bottom: parent.verticalCenter
            anchors.bottomMargin: 20
            font.pixelSize: 30
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    muteFlag = !muteFlag;
                    radio.setMuteFlag(muteFlag)
                    radio.setFrequency(fmValue.text)
                }
            }
        }

        Text {
            id: fmValue
            color: "white"
            anchors.horizontalCenter : parent.horizontalCenter
            anchors.bottom: parent.verticalCenter
            anchors.bottomMargin: 20
            font.pixelSize: 100
        }

        Image {
            id: slidebg
            source: "qrc:/radio/images/sliderbg.png"
            anchors.horizontalCenter : parent.horizontalCenter
            anchors.bottom: parent.verticalCenter
            anchors.bottomMargin: 120
            width: 800
            height: 68
        }
        Slider {
            id: slider
            anchors.horizontalCenter : parent.horizontalCenter
            anchors.bottom: parent.verticalCenter
            anchors.bottomMargin: 120
            width: 800
            height: 68
            value: 0.553125
            stepSize: 0.003125
            onValueChanged: {
                fmValue.text = String(slider.value*32+76).indexOf(".") != -1
                        ? String(slider.value*32+76).slice(0,5)
                        : String(slider.value*32+76).slice(0,5) + ".0"
                radio.setMuteFlag(muteFlag)
                radio.setFrequency(fmValue.text)
                //console.log(fmValue.text)
            }

            style:SliderStyle{
                groove:Rectangle{
                    color: "transparent"
                }
                handle: Rectangle{
                    anchors.centerIn: parent

                    Image {
                        id: handleImage
                        source: "qrc:/radio/images/handle.png"
                        anchors.centerIn: parent
                        opacity: control.pressed ? 0.8 : 1
                    }

                    width: 80
                    height: 80
                    color: "transparent"
                }
            }
        }

        ListModel {
            id: fmModel
            ListElement {name: ""}
            ListElement {name: ""}
            ListElement {name: ""}
            ListElement {name: ""}
            ListElement {name: ""}
            ListElement {name: ""}
            function getIcon(index, id) {
                var data = [
                            ["qrc:/radio/images/button.png"],
                            ["qrc:/radio/images/button.png"],
                            ["qrc:/radio/images/button.png"],
                            ["qrc:/radio/images/button.png"],
                            ["qrc:/radio/images/button.png"],
                            ["qrc:/radio/images/button.png"],
                        ]
                return data[index][0];
            }

            function getAppName(index, id) {
                var data = [
                            ["93.6MHz"],
                            ["95.4MHz"],
                            ["100.5MHz"],
                            ["103.6MHz"],
                            ["105.6MHz"],
                            ["107.7MHz"],
                        ]
                return data[index][0];
            }
        }

        GridView {
            id: fm_gridView
            visible: true
            anchors.top: parent.verticalCenter
            anchors.topMargin: 80
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            focus: true
            interactive: false
            cellWidth: fm_gridView.width / 3
            cellHeight: (fm_gridView.height) / 2
            model: fmModel
            delegate: delegateThumbnail

        }
        Component {
            id: delegateThumbnail
            Rectangle {
                id: rectangel
                width: fm_gridView.width / 3
                height: fm_gridView.cellHeight
                color: "transparent"

                Rectangle {
                    width: rectangel.width - 20
                    height: 60
                    radius: 8
                    color: "#0195de"
                    anchors.centerIn: parent
                    opacity: fm_gridView.currentIndex == index ? iconsOpacity : 1
                    Image {
                        id: browse_img
                        visible: false
                        antialiasing: true
                        source: fm_gridView.model.getIcon(index, 0)
                        anchors.fill: parent
                        fillMode: Image.Stretch
                        opacity: fm_gridView.currentIndex == index ? iconsOpacity : 1
                    }
                    Text {
                        anchors.centerIn: parent
                        id: appName
                        text: qsTr(fm_gridView.model.getAppName(index, 0))
                        color: "white"
                        font.pixelSize: 30
                        font.bold: true
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            fm_gridView.currentIndex = index
                            var tmp = appName.text.split("MHz")
                            slider.value = (parseFloat(tmp) - 76) * 0.03125
                            //console.log(parseFloat(tmp) + " " + slider.value)
                        }
                        onEntered: {
                            fm_gridView.currentIndex = index
                            iconsOpacity = 0.5
                        }
                        onPressed:{
                            iconsOpacity = 0.8
                            fm_gridView.currentIndex = index
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
