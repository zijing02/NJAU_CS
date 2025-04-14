/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   wireless
* @brief         MyWireless.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-01
*******************************************************************/
import wirelessModel 1.0
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11
import QtQuick.VirtualKeyboard 2.2
import "../helpbutton"

Item {
    id: myWireless
    visible: false
    property int mystate
    property bool wirelessonFlag: true

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/wireless.png"
        appName: "Wireless"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件为无线联网软件，使用前请插上正点原子店铺在售的Linux专用的USB WIFI（ALPHA开发板有赠送，软件默认支持的是USB WIFI，使用SDIO WIFI请自行修改软件），\
点击无线的名称，弹出输入框后，若热点有密码请输入8位以上的无线密码，再点击连接。若热点无密码，则直接点击连接即可。
2.连接成功则会显示连接“已连接”，未连上则会显示密码错误/已断开，在软件运行的过程如果连接好4次以上连接不成功，则程序不会再去连接！程序仅供参考与学习！
3.由于可能程序问题或者物理外界因素不可抗拒，若连接不上，请参考正点原子快速体验文档测试USB WIFI即可。"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本\n2.【Version 1.0.1 2021-03-25】。\n添加连接不上的说明。"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }
    Component.onCompleted: {
        myWireless.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: myWireless
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

    WirelessListModel {
        id: myWirelessListModel
        onCurrentIndexChanged: {
            playList.currentIndex = currentIndex
        }
        onReturnMainWinWifiResult: {
            mystate = state
        }
        onWirelessReadyChanged: {
            if (readyFlag)
                wlanText.text = "WLAN"
        }
    }

    MyWifiKeyInput{
        id: wifiDialog
        x: 0
        y: 0
        height: parent.height
        width: parent.width
        onToggleConnet: {
            if (wifiDialog.pskValue != "")
                myWirelessListModel.getMyWifiSSIDPSK(wifiDialog.ssidValue, wifiDialog.pskValue)
            else
                myWirelessListModel.passwordLessConnection(wifiDialog.ssidValue)
        }
    }

    ListView  {
        id: playList
        visible: true
        anchors.top: parent.top
        anchors.topMargin: 135
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width
        orientation:ListView.Vertical
        clip: true
        z: 1
        spacing: 5
        model: myWirelessListModel
        delegate: Rectangle {
            id: itembg
            width: playList.width
            height: 50
            color: mouserArea.pressed ?  Qt.rgba(46, 46, 46, 0.1) : "transparent"
            MouseArea {
                id: mouserArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    mystate = 0
                    myWirelessListModel.currentIndex = index
                    //console.log(wifiName.text)
                    wifiDialog.ssidValue = wifiName.text
                    wifiDialog.message = "请输入" + (wifiName.text + "的密码")
                    wifiDialog.open()
                }
            }
            Text {
                id: wifiName
                verticalAlignment: Text.AlignVCenter
                height: 50
                anchors.right: parent.right
                anchors.rightMargin: 150
                anchors.left: wifiLevelIcon.right
                anchors.leftMargin: 20
                text:  title
                elide: Text.ElideRight
                color: mouserArea.pressed ? "#0a7ad8" : "white"
                font.pixelSize: 20
            }

            Image {
                id: wifiLevelIcon
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                width: 30
                height: 22
                source: wifilevl(level)
                function wifilevl(arg){
                    var intArg = parseInt(arg)
                    switch (intArg){
                    case 0:
                        return "qrc:/wireless/imges/level0.png"
                    case 1:
                        return "qrc:/wireless/imges/level1.png"
                    case 2:
                        return "qrc:/wireless/imges/level2.png"
                    case 3:
                        return "qrc:/wireless/imges/level3.png"
                    case 4:
                        return "qrc:/wireless/imges/level4.png"
                    case 5:
                        return "qrc:/wireless/imges/level5.png"
                    default:
                        return "qrc:/wireless/imges/level5.png"
                    }
                }
            }

            Text {
                id: connectStateText
                text: connectsate()
                anchors.left: wifiName.right
                anchors.leftMargin: 5
                anchors.right: parent.right
                horizontalAlignment:Text.AlignHCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "white"
                function connectsate(){
                    if (wifiDialog.ssidValue === wifiName.text) {
                        switch (mystate){
                        case 1:
                            return qsTr("已连接")
                        case 2:
                            return qsTr("密码错误/已断开")
                        case 3:
                            return qsTr("正在连接...")
                        default:
                            return qsTr("")
                        }
                    } else return qsTr("")
                }

            }
        }

    }

    Item {
        anchors.top: parent.top
        anchors.bottom: playList.top
        anchors.right: parent.right
        anchors.left: parent.left

        Text {
            id: wlanText
            width: parent.width - 100
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.left: parent.left
            anchors.leftMargin: 20
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 15
            text: qsTr("WLAN")
            color: "#27d7f0"
        }

        Text {
            id: refreshText
            width: 100
            anchors.verticalCenter: refreshBtn.verticalCenter
            anchors.left: wlanText.left
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 15
            text: qsTr("附近WLAN(此软件仅供参考，详细点击右上角“关于”)")
            color: "#27d7f0"
        }

        RoundButton {
            id: refreshBtn
            width: 50
            height: 50
            enabled: wlanSwitch.checked
            opacity: wlanSwitch.checked ? 1 : 0
            anchors.top: wlanSwitch.bottom
            anchors.horizontalCenter: wlanSwitch.horizontalCenter
            Image {
                id: refreshImage
                width: 20
                height: 20
                anchors.centerIn: parent
                source: "qrc:/wireless/imges/refresh.png"
            }
            background: Rectangle {
                anchors.fill: parent
                color: "transparent"
            }

            onClicked: {
                //if (wlanSwitch.checked) {
                refreshAnima.running = true
                wlanText.text = "Scanning..."
                //}
                if(playList.count != 0 && wlanSwitch.checked)
                    myWirelessListModel.remove(0, playList.count - 1)
            }
            RotationAnimator {
                id: refreshAnima
                target: refreshBtn
                from: 0
                to: 360
                duration: 1000
                //loops: Animation.Infinite
                running: false
                onRunningChanged: {
                    if (running === false) {
                        from = refreshBtn.rotation;
                        to = from+360;
                    }
                }
                onStopped: {
                    if (wlanSwitch.checked) {
                        myWirelessListModel.wirelessInit()
                    }
                }
            }
        }

        Switch{
            id: wlanSwitch
            hoverEnabled: true
            anchors.verticalCenter: wlanText.verticalCenter
            anchors.left: wlanText.right
            onPressed: {
                if (wirelessonFlag)
                    wlanText.text = "Scanning..."
                else
                    wlanText.text = "WLAN"
                wirelessonFlag = !wirelessonFlag
            }
            onToggled: {
                if(checked){
                    //console.log("ok")
                    //if(!WINenv)
                    myWirelessListModel.wirelessInit()
                }
                else {
                    if(playList.count != 0)
                        myWirelessListModel.remove(0, playList.count - 1)
                    if(!WINenv)
                        myWirelessListModel.wirelessClose()
                    mystate = 0
                    //console.log("no")
                }
            }
            //onCheckableChanged: {}
        }

        Rectangle {
            id: line
            height: 1
            color: "#27d7f0"
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 20
        }
    }

}
