/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   aflex
* @brief         Circularprogress.qml
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
    property real minimumValue: 0
    property real maximumValue: 360
    property real currentValue: dialVaule
    //property color secondaryColor: "blue"
    property real maxValue: 50

    onCurrentValueChanged:canvas.requestPaint()

    Canvas {
        id: canvas
        width: parent.width
        height: parent.height
        antialiasing: true

        property real centerWidth: width/2
        property real centerHeight: height/2
        property real radius: width/3

        // this is the angle that splits the circle in two arcs
        // first arc is drawn from 0 radians to angle radians
        // second arc is angle radians to 2*PI radians
        property real angle: (currentValue - minimumValue) / (maximumValue - minimumValue) * 2 * Math.PI

        // we want both circle to start / end at 12 o'clock
        // without this offset we would start / end at 9 o'clock
        property real angleOffset: -Math.PI / 2
        signal clicked()

        onPaint: {
            var ctx = getContext("2d")
            ctx.save()
            ctx.clearRect(0, 0, canvas.width, canvas.height)
            //if (mouseArea.pressed) {
            //    timer.running = true
            //}

            //bottom arc
            ctx.beginPath()
            ctx.lineWidth = 3
            ctx.strokeStyle = 'rgba(255,0,0,0.5)'
            ctx.strokeStyle = "transparent"
            ctx.arc(canvas.centerWidth,
                    canvas.centerHeight,
                    canvas.radius,
                    2,
                    7.4)
            ctx.stroke()

            //progress arc
            ctx.beginPath()
            ctx.lineWidth = 3
            var grd = ctx.createLinearGradient(0,0,640,0)
            grd.addColorStop(0,Qt.rgba(0,3,248,50))
            grd.addColorStop(1.0,"#39bb80")
            ctx.strokeStyle = grd
            ctx.fillStyle = grd

            ctx.arc(canvas.centerWidth,
                    canvas.centerHeight,
                    canvas.radius,
                    2,
                    2 + canvas.angle)
            ctx.stroke()

            ctx.restore()
        }

        Text {
            visible: false
            id: txt_progress
            anchors.centerIn: parent

            font.pixelSize: 16
            text: "0%"
            color: "red"
        }

        MouseArea {
            id: mouseArea

            anchors.fill: parent
            onClicked: canvas.clicked()
            onPressedChanged: canvas.requestPaint()
        }

        /*Timer{
            id: timer
            interval: 30
            running: false
            repeat: false
            onTriggered: {
                if(currentValue === 100) {
                    currentValue = 0
                    txt_progress.text = "0"
                }
                if(currentValue<maxValue){
                    currentValue += 1
                }
                else{
                    currentValue=0
                }

                txt_progress.text = currentValue.toString()+"%"
            }
        }*/
    }
}
