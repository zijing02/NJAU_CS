/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   wireless
* @brief         MyWifiKeyInput.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-01
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window  2.2
import QtQuick.VirtualKeyboard 2.2
import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4

Dialog {
    id: dialog
    x: 0
    y: 0
    background: Rectangle{
        color:"#cc404040"
    }
    height: parent.height
    width: parent.width
    modal: true

    property string pskValue: " "
    property string ssidValue: " "
    property string message: " "
    signal toggleConnet
    signal toggleCancel

    // This must have a layer of Item or Rectangle over the input box or the keyboard will not shrink back when clicked
    Rectangle{
        id: bg
        anchors.bottom: inputPanel.top
        anchors.bottomMargin: 50
        width: 300
        height: 200
        anchors.horizontalCenter: parent.horizontalCenter
        color: "transparent"

        MyTextField {
            id: input
            anchors.top:bg.top
            anchors.topMargin: 100

            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: message
            color: "white"
        }

        Button{
            Text {
                id: cancel
                text: qsTr("取消")
                anchors.centerIn: parent
                font.pixelSize: 15
            }
            anchors.top: input.bottom
            anchors.topMargin: 5
            anchors.left: input.left
            height: input.height
            width: input.width/2-5
            onClicked: {
                toggleCancel()
                dialog.close()
            }
            background: Rectangle{
                radius: 20
                height: parent.height
                width: parent.width
                color: "#eeeeeeee"
            }
        }

        Button{
            Text {
                id: enter
                text: qsTr("连接")
                anchors.centerIn: parent
                color: input.text.length < 8 ? "black" : "#0490f6"
                font.pixelSize: 15
            }
            anchors.top: input.bottom
            anchors.topMargin: 5
            anchors.right: input.right
            height: input.height
            width: input.width/2-5
            onClicked: {
                if(input.text.length >= 0){
                    pskValue = input.text
                    //console.log(pskValue)
                    toggleConnet()
                    dialog.close()
                }
            }

            background: Rectangle{
                radius: 20
                height: parent.height
                width: parent.width
                color: "#eeeeeeee"
            }
        }
    }

    InputPanel {
        id: inputPanel
        z: 50
        //x: -11
        x: -24
        y: dialog.height
        width: dialog.width
        visible: myWireless.visible
        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: dialog.height - inputPanel.height
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
