/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   aflex
* @brief         Aflex.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-25
*******************************************************************/
import QtQuick 2.9
import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import "../helpbutton"
Item {
    property int count : 0

    id: aircondition
    visible: false

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/aflex.png"
        appName: "智控空调"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件是一款虚拟空调控制器界面。点击可切换空调的模式等，实际应用由用户自行开发，程序仅供参考。"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
        if (parent.visible){
            if(!WINenv) {
                // TODO
            }
        }
    }
    Component.onCompleted: {
        aircondition.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: aircondition
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


    Text {
        id: dac_text
        text: qsTr("空调控制")
        anchors.centerIn: parent
        color: "white"
        font.pixelSize: 50
        font.bold: true
    }
    MyDial{
        id: myDial
        anchors.centerIn: parent
        onMyDialVauleChange: {
            if(!WINenv){
                // TODO
            }
        }
    }
    Button {
        id: modelBt
        width: 100
        height: 40
        anchors.bottom: myDial.bottom
        anchors.bottomMargin: 70
        anchors.horizontalCenter: parent.horizontalCenter
        background: Rectangle {
            anchors.fill: parent
            color: "transparent"
        }
        Text {
            id: model_text
            anchors.centerIn: parent
            font.pixelSize: 15
            color: "white"
            text: qsTr("模式 干燥")
        }
        onClicked: {
            count++
            if (count >= 3)
                count = 0
            switch (count) {
            case 0:
                model_text.text = "模式 干燥"
                break;
            case 1:
                model_text.text = "模式 冷风"
                break;
            case 2:
                model_text.text = "模式 辅热"
                break;
            }
        }

    }

}
