/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   system
* @brief         System.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-31
*******************************************************************/
import QtQuick 2.0
import "../helpbutton"

Item {
    visible: false
    id: system

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/system.png"
        appName: "系统信息"
        appVersion: "Version 1.0.1"
        subtoolText: "1.此软件为系统信息展示软件。显示系统的信息。"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本\n2.【Version 1.0.1 2021-01-07】。\n修复一些问题，如开机耳机没有声音
3.【Version 1.0.2 2021-03-25】。\n添加WIFI连上不上的说明，删除关机的功能"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }
    Component.onCompleted: {
        system.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: system
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
    Flickable {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: smallScreen ? parent.height + 208 : parent.height +20
        Image  {
            id: qui
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            source: "qrc:/system/images/qui.png"
        }

        Text {
            id: uiVersion
            text: qsTr("QUI Version")
            color: "white"
            font.pixelSize: 20
            anchors.left: qui.right
            anchors.top: qui.top
            anchors.topMargin: 25
            font.bold: true
        }

        Text {
            id: version
            text: qsTr("QUI 1.0.2 | Stable version")
            color: "#ccffffff"
            font.pixelSize: 15
            anchors.left: qui.right
            anchors.top: uiVersion.bottom
            anchors.topMargin: 5
            font.bold: true
        }

        Rectangle {
            id: line
            height: 1
            color: "white"
            anchors.top: qui.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 25
            anchors.right: parent.right
            anchors.rightMargin: 25
        }


        ListModel {
            id: systemModel
            ListElement {name: ""}
            ListElement {name: ""}
            ListElement {name: ""}
            ListElement {name: ""}

            function getIcon(index, id) {
                var data = [
                            ["images/cpu.png"],
                            ["images/memory.png"],
                            ["images/emmc.png"],
                            ["images/ratio.png"]
                        ]
                return data[index][0];
            }

            function getItemName(index, id) {
                var data = [
                            ["CPU"],
                            ["MEMORY"],
                            [myDesktop.getCoreBoardInfo() ? "FLASH EMMC" : "NAND FLASH"],
                            ["RESOLUTION"]
                        ]
                return data[index][0];
            }

            function getNature(index, id) {
                var data = [
                            ["IMX6ULL 800MHz"],
                            [myDesktop.getCoreBoardInfo() ? "512MB" : "256MB"],
                            [myDesktop.getCoreBoardInfo() ? "8.00GB" : "512MB"],
                            [system.width + " x " + system.height]
                        ]
                return data[index][0];
            }
        }

        GridView {
            id: systemView
            visible: true
            anchors.top: parent.top
            anchors.topMargin: 120
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            focus: true
            interactive: false
            cellWidth: systemView.width / 2
            cellHeight: (systemView.height) / 3
            model: systemModel
            delegate: systemDelegate
        }
        Component {
            id: systemDelegate
            Rectangle {
                width: systemView.width / 2
                height: systemView.cellHeight
                color: "transparent"
                Rectangle {
                    width: 84
                    height: 84
                    radius: 8
                    color: "transparent"
                    anchors.centerIn: parent
                    Image {
                        id: icon
                        antialiasing: true
                        source: systemView.model.getIcon(index, 0)
                        anchors.fill: parent
                        fillMode: Image.Stretch
                    }
                    Text {
                        anchors.top: icon.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        id: textInfo
                        text: qsTr(systemView.model.getItemName(index, 0))
                        color: "white"
                        font.pixelSize: 20
                        font.bold: true
                    }

                    Text {
                        anchors.top: textInfo.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        id: nature
                        text: qsTr(systemView.model.getNature(index, 0))
                        color: "#ccffffff"
                        font.pixelSize: 15
                        font.bold: true
                    }
                }
            }
        }
    }
}
