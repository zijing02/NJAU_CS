/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   wireless
* @brief         MyTextField.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-01
*******************************************************************/
import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

TextField {
    id: myTextField

    property color checkedColor: "#D5DBDB"

    signal doubleClicked(var/*MouseEvent*/ event)

    placeholderText: qsTr("请输入内容")
    font.pixelSize: 15
    font.weight: Font.Thin
    antialiasing: true

    background: Rectangle {
        implicitWidth: 213
        implicitHeight: 42
        radius: 8
        color: myTextField.enabled ? "transparent" : "#F4F6F6"
        border.color: myTextField.enabled ? myTextField.checkedColor : "#D5DBDB"
        border.width: 2
        opacity: myTextField.enabled ? 1 : 0.7

        layer.enabled: myTextField.hovered
        layer.effect: DropShadow {
            id: dropShadow
            transparentBorder: true
            color: myTextField.checkedColor
            samples: 10
        }
    }

    cursorDelegate: Rectangle {
        width: 1
        height: parent.height * 0.4
        color: myTextField.checkedColor
        visible: myTextField.focus

        Timer {
            interval: 600
            repeat: true
            running: myTextField.focus
            onRunningChanged: parent.visible = running
            onTriggered: parent.visible = !parent.visible
        }
    }

    onDoubleClicked: selectAll()

    // note: This signal was introduced in QtQuick.Controls 2.1 (Qt 5.8).
    onPressed: {
        _private.mouseEvent = event
        _private.isCheckDoubleClickedEvent++

        if (! _checkDoubleClickedEventTimer.running)
            _checkDoubleClickedEventTimer.restart()
    }

    // Private
    Item {
        id: _private
        property int isCheckDoubleClickedEvent: 0
        property var/*MouseEvent*/ mouseEvent

        Timer {
            id: _checkDoubleClickedEventTimer
            running: false
            repeat: false
            interval: 180
            onTriggered: {
                if (_private.isCheckDoubleClickedEvent >= 2) {
                    // Double Clicked Event
                    myTextField.doubleClicked(_private.mouseEvent)
                }

                stop()
                _private.isCheckDoubleClickedEvent = 0
            }
        }
    }
}
