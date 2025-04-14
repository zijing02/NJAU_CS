/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   tcpclient
* @brief         TcpClient.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-31
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.2
import mytcpclient 1.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick 2.12
import QtQuick.VirtualKeyboard 2.2
import "../helpbutton"

Item {
    visible: false
    id: window
    property string netWorkInfo: ""
    property string connectSate : "请先与服务端连接"

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" :  "qrc:/desktop/androidstyleicons/tcpclient.png"
        appName: "Tcp客户端"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件为tcp客户端，点击聊天中的人物头像可获取本机的ip信息，可断开服务端的连接，清空聊天记录等。
2.输入服务器的ip地址与监听的端口，点击连接即可连接服务端，然后就可以互相发信息聊天！"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }

    Component.onCompleted: {
        mainListview.model.insert(mainListview.model.count,
                                  {"detail":"您好！我是TCP客户端小可爱，请输入服务端的IP与端口，再点击连接，即可连接到服务端，点击我头像可查看本地IP信息",
                                      "messageBg":"qrc:/tcpclient/images/rightMessagebg.png",
                                      "fatherMirroring":false,"childrenMirroring":false,
                                      "headPhotoImage":"qrc:/tcpclient/images/gril.jpg",
                                      "messBackground":"#b8fa99"})
        mainListview.currentIndex = mainListview.count - 1
        netWorkInfo = tcpclient.getLocalIpAddress()
        window.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: window
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
    MyTcpclient{
        id: tcpclient
        onRecMessageChanged: {
            // listmodel.append({"detail":tcpclient.recMessage})
            mainListview.model.insert(mainListview.model.count,{"detail":tcpclient.recMessage,
                                          "messageBg":"qrc:/tcpclient/images/leftMessagebg.png",
                                          "fatherMirroring":true,"childrenMirroring":true,
                                          "headPhotoImage":"qrc:/tcpclient/images/boy.jpg",
                                          "messBackground":"#e3e3f8"})
            mainListview.currentIndex = mainListview.count - 1
        }
        onNewConnetChanged:{
            connectSate = "断开连接"
            mainListview.model.insert(mainListview.model.count,{"detail":tcpclient.newConnet,
                                          "messageBg":"qrc:/tcpclient/images/rightMessagebg.png",
                                          "fatherMirroring":false,"childrenMirroring":false,
                                          "headPhotoImage":"qrc:/tcpclient/images/gril.jpg",
                                          "messBackground":"#b8fa99"})
            mainListview.currentIndex = mainListview.count - 1
        }
    }

    ListView{
        add: Transition {
            NumberAnimation { properties: "x,y"; from: window.height; duration: 200 }
        }
        id: mainListview
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.bottom: bottomrect.top
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.right:parent.right
        highlightFollowsCurrentItem: true
        spacing: 30
        MouseArea{
            width: parent.width - 120
            height: parent.height
            anchors.centerIn: parent
            onClicked: textfield.focus = false
        }
        delegate: Component{
            Rectangle{
                id: message
                LayoutMirroring.enabled: fatherMirroring
                LayoutMirroring.childrenInherit: childrenMirroring
                height: label1.lineCount * 12 + 35
                implicitWidth: fitWidth(label1.text) > window.width * 0.2
                               ? Math.min(fitWidth(label1.text), window.width * 0.2) + 60 : label1.implicitWidth + 80
                color:"transparent"
                anchors.right: parent.right
                anchors.rightMargin: 0
                Image {
                    anchors.right: parent.right
                    width: 40
                    height: 40
                    id: headPhoto
                    source: headPhotoImage
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            onClicked: container.state == 'right' ? container.state = '' : container.state = 'right'
                        }
                    }
                }
                Rectangle{
                    id: myMessbg
                    width: parent.width - 60
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    color: messBackground
                    radius: 5
                    Item{
                        anchors.left: myMessbg.left
                        anchors.leftMargin: 10
                        anchors.right: myMessbg.right
                        anchors.rightMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        Text{
                            id: label1
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: fitWidth(label1.text) > window.width * 0.2 ? Text.WrapAnywhere:Text.NoWrap
                            text: detail
                            font.pixelSize: 12
                        }
                    }
                }
                BorderImage{
                    id: mebg
                    source:  messageBg
                    anchors.left: myMessbg.right
                    anchors.leftMargin: 0
                    width: 5
                    height: 10
                    anchors.verticalCenter: myMessbg.verticalCenter
                    smooth: true
                }
            }
        }
        model:ListModel{
            id:listmodel
            //            ListElement{
            //                detail:"请先连接服务器"
            //            }
        }
    }

    function fitWidth(text){
        return fontMetrics.advanceWidth(text)
    }

    function qmlsendMessage(){
        if(textfield.text != ""){
            //listmodel.append({"detail":textfield.text})
            mainListview.model.insert(mainListview.model.count,{"detail":textfield.text,
                                          "messageBg":"qrc:/tcpclient/images/rightMessagebg.png",
                                          "fatherMirroring":false,"childrenMirroring":false,
                                          "headPhotoImage":"qrc:/tcpclient/images/gril.jpg",
                                          "messBackground":"#b8fa99"})
            mainListview.currentIndex = mainListview.count - 1
            tcpclient.sendMessage(textfield.text)
        }
    }

    FontMetrics {
        id: fontMetrics
        //font.family: "Arial"
    }

    Rectangle{
        z: 55
        height: 40
        anchors.bottom: pannelbg.top
        //anchors.bottomMargin: bottomDistance
        id: bottomrect
        Row{
            Rectangle {
                height: bottomrect.height
                width: window.width * 0.4
                color: "transparent"
                Text {
                    id: ipText
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("IP地址:")
                    font.bold: true
                    font.pixelSize: 13
                    color: "white"
                }
                TextField{
                    id: ipTextfield
                    placeholderText: "ip"
                    height: bottomrect.height
                    anchors.verticalCenter: parent.verticalCenter
                    width: window.width * 0.15
                    anchors.left: ipText.right
                    font.pixelSize: 13
                    color: "white"
                    onFocusChanged: {
                        if(inputPanel.state == 'visible' )
                            container.state = ''
                    }
                    background: Rectangle{
                        color: "transparent"
                        Rectangle {
                            anchors.bottom: parent.bottom
                            width: parent.width
                            color: ipTextfield.focus ? "#27d7f0" : "gray"
                            height: ipTextfield.focus ? 2 : 1
                        }
                    }
                }
                Text {
                    id: port
                    anchors.left: ipTextfield.right
                    anchors.leftMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("端口:")
                    font.bold: true
                    font.pixelSize: 13
                    color: "white"
                }
                TextField{
                    id: portTextfield
                    font.pixelSize: 13
                    placeholderText: "port"
                    anchors.left: port.right
                    height: bottomrect.height
                    width: window.width * 0.05
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                    onFocusChanged: {
                        if(inputPanel.state == 'visible' )
                            container.state = ''
                    }
                    background: Rectangle{
                        color: "transparent"
                        Rectangle {
                            anchors.bottom: parent.bottom
                            width: parent.width
                            color: portTextfield.focus ? "#27d7f0" : "gray"
                            height: portTextfield.focus ? 2 : 1
                        }
                    }
                }
                Button{
                    anchors.left: portTextfield.right
                    anchors.leftMargin: 5
                    text:"连接"
                    height: bottomrect.height
                    width: window.width * 0.08
                    font.pixelSize: 13
                    onClicked: {
                        tcpclient.buttonConnectClicked(ipTextfield.text,portTextfield.text)
                    }
                }
            }

            TextField{
                id: textfield
                placeholderText: "输入要发消息内容"
                height: bottomrect.height
                width: window.width * 0.4
                Keys.enabled: true
                font.pixelSize: 13
                color: "white"
                Keys.onReturnPressed : qmlsendMessage()
                onFocusChanged: {
                    if(inputPanel.state == 'visible' )
                        container.state = ''
                }
                background: Rectangle{
                    color: "transparent"
                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        color: textfield.focus ? "#27d7f0" : "gray"
                        height: textfield.focus ? 2 : 1
                    }
                }
            }
            Rectangle{
                height: bottomrect.height
                width: window.width * 0.2
                color: "#55111111"
                Button{
                    anchors.fill: parent
                    text:"发送（或者按回车键）"
                    font.pixelSize: 13
                    onClicked: {
                        qmlsendMessage()
                    }
                }
            }
        }
    }
    Rectangle{
        z: 88
        id: pannelbg
        width: parent.width
        anchors.bottom: inputPanel.top
        height: inputPanel.state == "visible" ? 28 * window.height / 480 : 0
        color: "black"
    }
    InputPanel {
        id: inputPanel
        z: 88
        //x: -11
        y: window.height
        visible: parent.visible
        width: window.width
        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
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

    Item {
        id: container
        states: State {
            name: "right"
            PropertyChanges {
                target: grilInfo
                x: parent.width * (smallScreen ? 0.50 : 0.65)
            }
        }

        transitions: Transition {
            NumberAnimation {
                property: "x"
                easing.type: Easing.InOutQuart
                duration: 1000
            }
        }
    }
    Item {
        id: grilInfo
        z: 40
        width: parent.width * (smallScreen ? 0.50 : 0.35)
        height: parent.height
        x: parent.width
        Flickable {
            id: myflickable
            anchors.fill: parent
            contentWidth: parent.width
            contentHeight: parent.height + (smallScreen ? 208 : 20)
            Rectangle{
                anchors.fill: parent
                color: "#cc424242"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        onClicked: container.state == 'right' ? container.state = '' : container.state = 'right'
                    }
                }
            }

            Image {
                id: boyID
                source: "qrc:/tcpclient/images/gril.jpg"
                width: 50
                height: 50
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 30
            }
            Text {
                id: juese
                anchors.left: boyID.right
                anchors.leftMargin: 20
                anchors.top: boyID.top
                text: qsTr("角色:TCP客户端")
                color: "white"
                font.bold: true
                font.pixelSize: 15
            }
            Text {
                id: idName
                text: qsTr("I  D:小可爱")
                lineHeightMode: Text.FixedHeight //设置行间距 以像素的方式设置
                lineHeight: 18 //行间距比例 最大 1
                anchors.left: juese.left
                anchors.top: juese.bottom
                anchors.topMargin: 5
                color: "#99eeeeee"
                font.bold: true
                font.pixelSize: 14
            }
            Text {
                anchors.top: boyID.bottom
                anchors.topMargin: 30
                anchors.horizontalCenter: parent.horizontalCenter
                id: networkMess
                text: netWorkInfo
                color: "#28dcf7"
                font.bold: true
                font.pixelSize: 13
            }
            Button{
                id: refreshBtn
                anchors.top: networkMess.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: 40
                height: 40
                background: Rectangle{
                    color: "transparent"
                    Image {
                        //anchors.fill: parent
                        anchors.centerIn: parent
                        id: refresh
                        source: "qrc:/tcpclient/images/refresh.png"
                    }
                }
                onClicked: {
                    netWorkInfo = "正在读取本地网卡信息，请稍候..."
                    refreshAnima.running = true
                }
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
                    netWorkInfo = tcpclient.getLocalIpAddress()
                }
            }
            Button{
                id: clearMessage
                anchors.top: refreshBtn.bottom
                //anchors.topMargin: -20
                anchors.horizontalCenter: parent.horizontalCenter
                width: 120
                height: 35
                background: Rectangle{
                    color: "transparent"
                    radius: 20
                    border.color: "#28dcf7"
                    border.width: 1
                    Text {
                        anchors.centerIn: parent
                        id: clearText
                        text: qsTr("清空所有聊天记录")
                        font.pixelSize: 13
                        color: clearMessage.pressed ? "white":"#28dcf7"
                    }
                }
                onClicked: {
                    if(mainListview.count != 0)
                        mainListview.model.clear()
                    mainListview.model.insert(mainListview.model.count,{"detail":"已经清空！",
                                                  "messageBg":"qrc:/tcpclient/images/rightMessagebg.png",
                                                  "fatherMirroring":false,"childrenMirroring":false,
                                                  "headPhotoImage":"qrc:/tcpclient/images/gril.jpg",
                                                  "messBackground":"#b8fa99"})
                    mainListview.currentIndex = mainListview.count - 1
                }
            }

            Button{
                id: disConetFromClient
                anchors.top: clearMessage.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                width: 120
                height: 35
                background: Rectangle{
                    color: "transparent"
                    radius: 20
                    border.color: "#28dcf7"
                    border.width: 1
                    Text {
                        anchors.centerIn: parent
                        id: disConetText
                        text: connectSate
                        color: disConetFromClient.pressed ? "white":"#28dcf7"
                        font.pixelSize: 13
                    }
                }
                onClicked: {
                    tcpclient.disConnect()
                    connectSate = "已断开连接"
                }
            }

        }
    }
}
