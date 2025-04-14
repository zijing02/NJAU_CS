/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   sensor
* @brief         Sensor.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-10
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQml.Models 2.12
import ap3216c 1.0
import icm20608 1.0
import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "../helpbutton"

Item {
    property var strArray:[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    visible: false
    id: sensor

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/sensor.png"
        appName: "传感器"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件为传感器软件，程序获取并计算ap3216c及icm20608的数据显示在屏幕上。程序启动时自动获取，关闭页面，会停止获取，节约cpu开支。"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本\n2.【Version 1.0.1 2021-01-25】。\n修复六轴MEMS传感器加速度计Y轴写重复，应该为Z轴"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
        if(!WINenv){
            ap3216c.setCapture(sensor.visible)
            icm20608.setCapture(sensor.visible)
        }
    }
    Component.onCompleted: {
        sensor.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: sensor
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

    ListModel {
        id: m_model
        ListElement {
            title: "3&1环境传感器AP3216C环境光强度(ALS):"
            itemValue: "0"
        }
        ListElement {
            title: "3&1环境传感器AP3216C接近距离(PS):"
            itemValue: "0"
        }
        ListElement {
            title: "3&1环境传感器AP3216C红外线强度(IR):"
            itemValue: "0"
        }
        ListElement {
            title: "六轴MEMS传感器陀螺仪X轴(GX):"
            itemValue: "0"
        }
        ListElement {
            title: "六轴MEMS传感器陀螺仪Y轴(GY):"
            itemValue: "0"
        }
        ListElement {
            title: "六轴MEMS传感器陀螺仪Z轴(GZ):"
            itemValue: "0"
        }
        ListElement {
            title: "六轴MEMS传感器加速度计X轴(AX):"
            itemValue: "0"
        }
        ListElement {
            title: "六轴MEMS传感器加速度计Y轴(AY):"
            itemValue: "0"
        }
        ListElement {
            title: "六轴MEMS传感器加速度计Z轴(AZ):"
            itemValue: "0"
        }
        ListElement {
            title: "六轴MEMS传感器温度值(TEMP):"
            itemValue: "0"
        }
    }

    function setValue() {
        for (var i = 0; i < m_model.count; i++) {
            var item = m_model.get(i);
            item.itemValue = strArray[i]
        }
    }

    Ap3216c {
        id: ap3216c
        onAp3216cDataChanged: {
            strArray[0] = alsData
            strArray[1] = psData
            strArray[2] = irData
        }
    }

    Icm20608 {
        id: icm20608
        onIcm20608DataChanged: {
            strArray[3] = gxData + "°/S"
            strArray[4] = gyData + "°/S"
            strArray[5] = gzData + "°/S"
            strArray[6] = axData + "g"
            strArray[7] = ayData + "g"
            strArray[8] = azData + "g"
            strArray[9] = tempData + "℃"
            setValue()
        }
    }

    ListView {
        id: listview
        width: parent.width
        anchors.centerIn: parent
        height: parent.height - 40
        clip: true
        model: m_model
        delegate:Rectangle {
            height: 50
            width: parent.width
            color: index % 2 == 0? "transparent" : "#304d87ee"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listview.currentIndex = index
                }
            }
            Text {
                id: functionText
                text: title
                color: "white"
                font.pixelSize: smallScreen ? 13 : 15
                anchors.left: parent.left
                width: parent.width / 2
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle {
                width: 1
                id: line
                height: parent.height - 5
                anchors.centerIn: parent
                color: "gray"
            }
            Text {
                id: valueTex
                text: itemValue
                anchors.left: line.right
                anchors.leftMargin: 15
                anchors.verticalCenter: parent.verticalCenter
                color: "#ffec57"
                font.pixelSize: 15
                font.bold: true
            }
        }
    }
}
