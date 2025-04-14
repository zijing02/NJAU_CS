/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11
import Qt.labs.calendar 1.0
import QtQuick.VirtualKeyboard 2.2
import "../helpbutton"
import "../desktop"

Item {
    property string myString: ""
    id: window
    visible: false

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/time.png"
        appName: "闹钟"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件可以设置闹钟数据，可设置一周内重复的天数，及添加备注，但是实际闹钟不会响铃，因为没做这部分功能，需要可自行改写。
2.记录闹钟数据，能保存到数据库，用户设置后需要点击主界面关机、重启、退出按钮才能保存数据，直接断电或者按复位键是不能保存所修改的数据的！程序未设计动态保存写入的数据，可自行修改！"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }
    Component.onCompleted: {
        window.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: window
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

    SwipeView {
        id: swipeView_app
        anchors.fill: parent
        currentIndex: 0
        Item {
            Clock{
                id: myClock
                anchors.centerIn: parent
            }
            Text {
                id: name
                text: currentTimeStringSecond
                anchors.top: myClock.bottom
                anchors.topMargin: smallScreen ? -50 : 0
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.pixelSize: 50
                font.bold: true
            }
        }
        Item {
            ListView {
                id: alarmListView
                anchors.top: parent.top
                anchors.topMargin: 35
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                model: AlarmModel {}
                delegate: AlarmDelegate {}
            }

            RoundButton {
                id: addAlarmButton
                text: "+"
                anchors.bottom: alarmListView.bottom
                anchors.bottomMargin: 8
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: alarmDialog.open()
            }

            AlarmDialog {
                id: alarmDialog
                x: Math.round((parent.width - width) / 2)
                y: Math.round((parent.height - height) / 2)
                alarmModel: alarmListView.model
            }
            TextField{
                id: myInputText
                anchors.bottom : parent.verticalCenter
                anchors.bottomMargin: 50
                anchors.horizontalCenter: parent.horizontalCenter
                visible: inputPanel.state == "visible"
                placeholderText: qsTr("请输入备注信息")
                text: alarmListView.model.get(alarmListView.currentIndex).label
                onTextChanged: alarmListView.model.get(alarmListView.currentIndex).label = text
                background: Rectangle{
                    color: "transparent"
                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        color: myInputText.focus ? "#27d7f0" : "gray"
                        height: myInputText.focus ? 2 : 1
                    }
                }
            }

            InputPanel {
                id: inputPanel
                z: 88
                y: window.height
                width: window.width
                visible: window.visible
                states: State {
                    name: "visible"
                    when: inputPanel.active
                    PropertyChanges {
                        target: inputPanel
                        y: window.height - inputPanel.height
                    }
                }
                transitions: Transition {
                    from: ""
                    to: "visible"
                    reversible: true
                    ParallelAnimation {
                        NumberAnimation {
                            properties: "y"
                            duration: 250
                            easing.type: Easing.InOutQuad
                        }
                    }
                }
            }
        }
    }
}
