/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   iotest
* @brief         Iotest.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-20
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import beep 1.0
import "../helpbutton"

Item {
    property bool beepFlag: false
    visible: false
    id: iotest

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/iotest.png"
        appName: "按键测试"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件是测试底板上的按钮与蜂鸣器所用，按下KEY0按键蜂鸣器响。"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
        beep.setKeyMonitor(iotest.visible)
    }
    Component.onCompleted: {
        iotest.x = parent.width
    }
    onFocusChanged: keyItem.forceActiveFocus()
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: iotest
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

    Beep {
        id: beep
        onKeysPressChanged: {
            if (beep.key0Str)
                console.log("Key0 Pressed!")
            if (!beep.key0Str)
                beepFlag = false
            else
                beepFlag = true
        }
    }
    Flickable {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: parent.height +20
        Button {
            id: key0
            width: 120
            height: 50
            anchors.centerIn: parent
            checkable: true
            Text {
                text: qsTr("KEY0")
                color: "white"
                anchors.centerIn: parent
                font.pixelSize: 20
                font.bold: true
            }
            style: ButtonStyle {
                background: Rectangle {
                    anchors.fill: parent
                    color:  (!WINenv ? beep.key0Str : 0) ||  key0.checked ? "#0263b2" : "#0195de"
                    radius: 8
                }
            }
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    key0.checked = true
                    if (!WINenv)
                        beep.setBeepState(true)
                }
                onReleased: {
                    key0.checked = false
                    if (!WINenv)
                        beep.setBeepState(false)
                }
            }
        }
    }

    Item {
        id: keyItem
        anchors.fill: parent
        Keys.enabled: true
        focus: true
        Keys.onPressed:  {
            if (event.key == Qt.Key_Up || event.key == Qt.Key_VolumeDown){
                beepFlag = true
                if(!WINenv)
                    beep.setBeepState(true)
            }

            if (event.key == Qt.Key_Down || event.key == Qt.Key_VolumeUp ){
                beepFlag = true
                if(!WINenv)
                    beep.setBeepState(true)
            }

        }
        Keys.onReleased: {
            if (event.key == Qt.Key_Up || event.key == Qt.Key_VolumeDown
                    || event.key == Qt.Key_Down || event.key == Qt.Key_VolumeUp ) {
                beepFlag = false
                if(!WINenv)
                    beep.setBeepState(false)
            }
        }
    }
}
