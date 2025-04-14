/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   aflex
* @brief         CustomDial.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-25
*******************************************************************/
import QtQuick 2.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
Dial {
    id: dial
    Text {
        anchors.centerIn: parent
        id: value_text
        color:"white"
        font.pixelSize: 50
        text: dial.value.toString()
    }
    Text {
        id: unit_text
        text: qsTr("mV")
        anchors.bottom: value_text.bottom
        color: "white"
        anchors.left: value_text.right
        font.bold: true
        font.pixelSize: 15
    }

    // Please set tickmarksVisible false, or your app will be slow to initialize
    tickmarksVisible: false
    stepSize: 1
    maximumValue: 3300
    value: 50
    minimumValue: 0
    width: 200
    height: 200

    onValueChanged:{
        dialVaule = dial.value
        emit: myDialVauleChange()
    }

    style: DialStyle {
        id:dialStyle
        // A small scale between a large scale,
        // Please do not use these two parameters, or your app will be slow to initialize
        // minorTickmarkCount: 10
        // minorTickmarkInset: 0
        minorTickmark:Rectangle{
            visible: false
            color:"blue"
            implicitWidth: 5
            implicitHeight: 5
            radius: implicitWidth/2
            MouseArea{
                enabled: false
                anchors.fill: parent
                onClicked: {
                    console.log("value"+styleData.value)
                    console.log("index"+styleData.index)
                    console.log(valueToAngle(styleData.value))
                }
            }
        }

        // Scale (the point at which a small scale is separated)
        tickmark:Rectangle{
            visible: false
            implicitWidth: 10
            implicitHeight: 10
            color: "yellow"
            radius: implicitWidth/2
            MouseArea{
                enabled: false
                anchors.fill: parent
                onClicked: {
                    console.log("value"+styleData.value)
                    console.log("index"+styleData.index)
                    console.log(valueToAngle(styleData.value))
                }
            }
        }
        tickmarkInset: -5
        tickmarkStepSize:2
        tickmarkLabel:Rectangle{
            visible: false
            color: "red"
            implicitWidth: 5
            implicitHeight: 5
            radius: implicitWidth/2
        }
        labelStepSize: 2
        labelInset: -10
        handle :Rectangle{
            color:"transparent"
            height: 30
            width: 30
            radius: width/2
        }
        background: Rectangle{
            color:"gray"
            radius: width/2
        }
    }
}
