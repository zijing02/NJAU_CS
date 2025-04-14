/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

import QtQuick 2.0
import "calculator.js" as CalcEngine
import "../helpbutton"

Item {
    id: calculator
    visible: false

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/calculator.png"
        appName: "计算器"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件为计算器软件，输入数字可进入简单的运算
2.拖动结果栏可查看记录的运算结果，点击结果栏则清空以前的记录"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }
    Component.onCompleted: {
        calculator.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: calculator
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

    function operatorPressed(operator) {
        CalcEngine.operatorPressed(operator)
        numPad.buttonPressed()
    }
    function digitPressed(digit) {
        CalcEngine.digitPressed(digit)
        numPad.buttonPressed()
    }
    function isButtonDisabled(op) {
        return CalcEngine.disabled(op)
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_0)
            digitPressed("0")
        else if (event.key == Qt.Key_1)
            digitPressed("1")
        else if (event.key == Qt.Key_2)
            digitPressed("2")
        else if (event.key == Qt.Key_3)
            digitPressed("3")
        else if (event.key == Qt.Key_4)
            digitPressed("4")
        else if (event.key == Qt.Key_5)
            digitPressed("5")
        else if (event.key == Qt.Key_6)
            digitPressed("6")
        else if (event.key == Qt.Key_7)
            digitPressed("7")
        else if (event.key == Qt.Key_8)
            digitPressed("8")
        else if (event.key == Qt.Key_9)
            digitPressed("9")
        else if (event.key == Qt.Key_Plus)
            operatorPressed("+")
        else if (event.key == Qt.Key_Minus)
            operatorPressed("−")
        else if (event.key == Qt.Key_Asterisk)
            operatorPressed("×")
        else if (event.key == Qt.Key_Slash)
            operatorPressed("÷")
        else if (event.key == Qt.Key_Enter || event.key == Qt.Key_Return)
            operatorPressed("=")
        else if (event.key == Qt.Key_Comma || event.key == Qt.Key_Period)
            digitPressed(".")
        else if (event.key == Qt.Key_Backspace)
            operatorPressed("backspace")
    }

    Display {
        id: display
        height: 80
        anchors.horizontalCenter: parent.horizontalCenter
        width: numPad.width
        anchors.bottom: numPad.top
    }
    Rectangle {
        id: numberBg
        anchors.fill: numPad
        color: "black"

    }
    NumberPad {
        id: numPad
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
