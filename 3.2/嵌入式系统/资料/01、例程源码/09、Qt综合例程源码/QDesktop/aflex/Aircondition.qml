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
import adc 1.0
import dac 1.0
import "../helpbutton"
Item {
    property string adcDataText
    property int dacValue
    property bool flag: false

    id: aflex
    visible: false

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/aflex.png"
        appName: "电压测量"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件是一路ADC数据采集，一路DAC数据输出。ADC数据输出可提供给DAC采集，注意采集范围不要大于±3.3v,否则可能损坏cpu。
2.使用方法是首先点击左边的Start，ADC就会开始采集，采集的数据可来自DAC的输出或者是可微调电位器的电压。请通过底板上的跳线帽来切换要采集的电压。退出程序会自动关闭采集。"
        versionText: "1.【Version 1.0.0】。\n初始版本"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
        if (parent.visible){
            flag = false
            if(!WINenv)
                adc.setCapture(flag)
            else
                adcDataText = "0.0"
        }
    }
    Component.onCompleted: {
        aflex.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: aflex
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

    Adc{
        id: adc
        onAdcDataChanged: {
            adcDataText = adcData
        }
    }

    Dac{
        id : dac
    }

    Text {
        id: dac_text
        text: qsTr("DAC")
        anchors.bottom: myDial.top
        anchors.bottomMargin: -20
        anchors.horizontalCenter: myDial.horizontalCenter
        color: "white"
        font.pixelSize: 50
        font.bold: true
    }
    MyDial{
        id: myDial
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 30
        anchors.verticalCenter: parent.verticalCenter
        onMyDialVauleChange: {
            if(!WINenv){
                dacValue = dialVaule / 0.805664062;
                dac.setDac(dacValue)
            }
        }
    }

    Text {
        id: adc_text
        text: qsTr("ADC")
        anchors.bottom: adcbg.top
        anchors.bottomMargin: 20
        anchors.horizontalCenter: adcbg.horizontalCenter
        color: "white"
        font.pixelSize: 50
        font.bold: true
    }
    Image {
        id: adcbg
        width: 230
        height: 230
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 30
        anchors.verticalCenter: parent.verticalCenter
        source: "qrc:/aflex/images/dialbg.png"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                flag = !flag
                if(!WINenv)
                    adc.setCapture(flag)
            }
        }
        Rectangle{
            anchors.centerIn: parent
            width: 200
            height: 200
            radius: width / 2
            color: "gray"
            Text {
                id: adcText
                text: flag ? adcDataText : "Start"
                color: "white"
                font.pixelSize: 50
                anchors.centerIn: parent
            }
            Text {
                id: unit_text
                text: flag ? qsTr("mV") : ""
                anchors.bottom: adcText.bottom
                color: "white"
                anchors.left: adcText.right
                font.bold: true
                font.pixelSize: 15
            }
        }
    }
}
