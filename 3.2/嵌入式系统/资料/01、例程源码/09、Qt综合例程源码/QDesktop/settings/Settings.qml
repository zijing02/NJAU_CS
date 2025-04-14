import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import QtQuick.Controls 1.2
import "../helpbutton"
import myDesktop 1.0

Item {
    id: settings
    visible: false

    HelpMessages {
        anchors.fill: parent
        z: 105
        appName: "设置"
        appVersion: "Version 1.0.1"
        subtoolText: "1.此软件是设置媒体音量（扬声器的音量），及点击关机、重启和退出整个App桌面，注意6U使用关机指令需要长按4G模块接口旁边的ON_OFF按钮才重启。"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本\n2.【Version 1.0.1 2021-01-07】。\n修复开机耳机没有声音，及音量条70%以下喇叭就没有声音的问题\
\n3.【Version 1.0.2 2021-03-25】。\n删除关机功能，避免用户关机，没法启动"
    }
    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }
    Component.onCompleted: {
        settings.x = parent.width
    }

    MyDesktop {
        id: myDesktop
        onSysVolumeChanged: {
            system_volume_slider.value = Number(sysVolume)
        }
    }

    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: settings
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

    Dialog {
        id: exitDialog
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        background: Rectangle {
            anchors.fill: parent
            color: "#1f1e58"
            Text {
                id: name
                text: qsTr("桌面已退出!重启桌面在串口终端输入\n/opt/QDesktop >/dev/null 2>&1 &")
                color: "white"
                anchors.centerIn: parent
                font.pixelSize: 15
                font.bold: true
            }
        }
    }

    Timer {
        id: timerClose
        interval: 1000
        repeat: false
        running: false
        onTriggered: {
            Qt.quit()
        }
    }

    Flickable {
        id : flickable
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: smallScreen ? parent.height + 208 : parent.height + 20
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
        Text {
            id: settingsText
            anchors.top: parent.top
            anchors.topMargin: 60
            anchors.left: parent.left
            anchors.leftMargin: 20
            text: qsTr("设置")
            color: "white"
            font.pixelSize: 25
        }

        Image {
            id: bellImage
            source: "qrc:/settings/images/bell.png"
            anchors.right: volumeText.left
            anchors.rightMargin: 10
            opacity: 0.7
            anchors.verticalCenter: volumeText.verticalCenter
        }

        Text {
            id: volumeText
            text: qsTr("扬声器(喇叭)音量音量设置")
            color: "white"
            font.pixelSize: 15
            anchors.top: settingsText.bottom
            anchors.topMargin: 30
            anchors.left: parent.left
            anchors.leftMargin: 80
        }

        Slider {
            id: system_volume_slider
            height: 50
            width: 280
            anchors.top: volumeText.bottom
            anchors.topMargin: 30
            anchors.left: parent.left
            anchors.leftMargin: 50
            updateValueWhileDragging: true
            stepSize: 1
            minimumValue: 80
            maximumValue: 127
            property bool handled: false
            onPressedChanged: {
                handled = !handled
                if (!handled)
                    myDesktop.setSystemVolume(value)
            }
            style: SliderStyle {
                groove: Rectangle {
                    id:kk
                    width: control.width
                    height: 30
                    radius: 8
                    color: "gray"
                    Rectangle {
                        width: styleData.handlePosition
                        height: 30
                        color: "#27e0fb"
                        radius: 8
                    }
                }

                handle: Rectangle {
                    width: 1
                    height: 30
                    color: "transparent"
                    Text {
                        id: pencent
                        anchors.bottom: parent.top
                        color: "#27e0fb"
                        font.pixelSize: 18
                        text: String(((control.value - 80) / 47 * 100).toFixed(1))/*.slice(0,2)*/ + "%"
                    }
                }
            }
        }

        ListModel {
            id: systemModel
            ListElement {name: ""}
            ListElement {name: ""}
            ListElement {name: ""}

            function getIcon(index, id) {
                var data = [
                            ["images/close.png"],
                            ["images/poweroff.png"],
                            ["images/reboot.png"],
                        ]
                return data[index][0];
            }

            function getItemName(index, id) {
                var data = [
                            ["退出桌面"],
                            ["关机(已禁用)"],
                            ["重启"],
                        ]
                return data[index][0];
            }

            function getNature(index, id) {
                var data = [
                            ["您将退出桌面程序"],
                            ["请您直接关闭电源"],
                            ["您的系统将会重启"],
                        ]
                return data[index][0];
            }
        }

        GridView {
            id: settingsView
            visible: true
            anchors.top: system_volume_slider.bottom
            anchors.topMargin: 80
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            focus: true
            //interactive: false
            cellWidth: settingsView.width / 3
            cellHeight: (settingsView.height) / 3
            model: systemModel
            delegate: systemDelegate
        }
        Component {
            id: systemDelegate
            Rectangle {
                width: settingsView.width / 3
                height: settingsView.cellHeight
                color: "transparent"
                Rectangle {
                    id :rectangle
                    width: 150
                    height: 150
                    radius: 8
                    color: "#aabbbbbb"
                    anchors.centerIn: parent
                    Image {
                        id: icon
                        antialiasing: true
                        source: settingsView.model.getIcon(index, 0)
                        //anchors.fill: parent
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        anchors.top: parent.top
                        anchors.topMargin: 20
                        fillMode: Image.Stretch
                    }
                    Text {
                        anchors.top: icon.bottom
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        anchors.topMargin: 20
                        id: textInfo
                        text: qsTr(settingsView.model.getItemName(index, 0))
                        color: "white"
                        font.pixelSize: 20
                        font.bold: true
                    }

                    Text {
                        anchors.top: textInfo.bottom
                        anchors.topMargin: 20
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        id: nature
                        text: qsTr(settingsView.model.getNature(index, 0))
                        color: "#ccffffff"
                        font.pixelSize: 15
                        font.bold: true
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            settingsView.currentIndex = index
                            switch (settingsView.currentIndex) {
                            case 0:
                                exitDialog.open()
                                timerClose.start()
                                break;
                            case 1:
                                poweroffFlag = true
                                //Qt.quit()
                                break;
                            case 2:
                                rebootFlag = true
                                Qt.quit()
                                break;
                            }
                        }
                        onEntered: {
                            settingsView.currentIndex = index
                            //rectangle.opacity = 0.5
                        }
                        onPressed:{
                            rectangle.opacity = 0.8
                            settingsView.currentIndex = index
                        }
                        onReleased:{
                            rectangle.opacity = 1
                        }
                        onExited: rectangle.opacity = 1
                    }
                }
            }
        }
    }
}
