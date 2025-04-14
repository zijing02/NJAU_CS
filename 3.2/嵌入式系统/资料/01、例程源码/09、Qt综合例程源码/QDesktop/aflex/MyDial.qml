/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   aflex
* @brief         MyDial.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-25
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
Item {
    property int dialVaule: 0
    signal myDialVauleChange
    width: 300
    height: 300
    Image {
        width: 230
        height: 230
        id: dialbg
        anchors.centerIn: parent
        source: "qrc:/aflex/images/dialbg.png"
    }
    CircularProgress{
        id: circularprogress
        anchors.centerIn: parent
        width: 310
        height: 310
    }
    CustomDial {
        visible: true
        width: 200
        height: 200
        anchors.centerIn: parent
        value: dialVaule
    }
}
