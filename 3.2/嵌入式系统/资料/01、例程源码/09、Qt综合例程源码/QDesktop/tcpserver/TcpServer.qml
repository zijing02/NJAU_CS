/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   tcpserver
* @brief         TcpServer.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-31
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.2
import mytcpserver 1.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick 2.12
import QtQuick.VirtualKeyboard 2.2
import "../helpbutton"

Item {
    visible: false
    id: window
    property int bottomDistance: 0
    property string netWorkInfo: ""
    property string connectSate : "请先与客户端连接"

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/tcpserver.png"
        appName: "Tcp服务端"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件为tcp服务器，点击聊天中的人物头像可获取本机的ip信息，可断开服务端的连接，清空聊天记录等。
2.请先用本机上的客户端连接或者手机下载一个tcp通信软件连接该软件，连上后就可以互相发信息聊天！"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }

    Component.onCompleted: {
        mainListview.model.insert(mainListview.model.count,{"detail":"您好！我是TCP服务端小胖子，请先用客户端连接，点击我头像可获取连接信息",
                                      "messageBg":"qrc:/tcpserver/images/rightMessagebg.png",
                                      "fatherMirroring":false,"childrenMirroring":false,
                                      "headPhotoImage":"qrc:/tcpserver/images/boy.jpg",
                                      "messBackground":"#b8fa99"})
        mainListview.currentIndex = mainListview.count - 1
        netWorkInfo = tcpserver.getLocalIpAddress()
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
    MyTcpserver{
        id: tcpserver
        onRecMessageChanged: {
            // listmodel.append({"detail":tcpserver.recMessage})
            mainListview.model.insert(mainListview.model.count,{"detail":tcpserver.recMessage,
                                          "messageBg":"qrc:/tcpserver/images/leftMessagebg.png",
                                          "fatherMirroring":true,"childrenMirroring":true,
                                          "headPhotoImage":"qrc:/tcpserver/images/gril.jpg",
                                          "messBackground":"#e3e3f8"})
            mainListview.currentIndex = mainListview.count - 1
        }
        onNewConnetChanged:{
            connectSate = "断开连接"
            mainListview.model.insert(mainListview.model.count,{"detail":tcpserver.newConnet,
                                          "messageBg":"qrc:/tcpserver/images/rightMessagebg.png",
                                          "fatherMirroring":false,"childrenMirroring":false,
                                          "headPhotoImage":"qrc:/tcpserver/images/boy.jpg",
                                          "messBackground":"#b8fa99"})
            mainListview.currentIndex = mainListview.count - 1
        }
    }

    ListView{
        add: Transition {
            // 设置增加Item时的动画 from 100,100  to不设置 就是默认在ListView
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
                LayoutMirroring.enabled: fatherMirroring
                LayoutMirroring.childrenInherit: childrenMirroring
                height: label1.lineCount * 12 + 35
                implicitWidth:  fitWidth(label1.text) > window.width * 0.2
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
                            // state属性值为空字符串时（''）即默认状态
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
                    color: messBackground //"#b8fa99"
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
                            wrapMode: fitWidth(label1.text) >= window.width*0.2 ? Text.WrapAnywhere:Text.NoWrap
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
                                          "messageBg":"qrc:/tcpserver/images/rightMessagebg.png",
                                          "fatherMirroring":false,"childrenMirroring":false,
                                          "headPhotoImage":"qrc:/tcpserver/images/boy.jpg",
                                          "messBackground":"#b8fa99"})
            mainListview.currentIndex = mainListview.count - 1
            tcpserver.sendMessage(textfield.text)
        }
    }

    FontMetrics {
        id: fontMetrics
        //font.family: "Arial"
    }

    // 底部
    Rectangle{
        z: 55
        height: 40
        anchors.bottom: pannelbg.top
        //anchors.bottomMargin: bottomDistance
        id: bottomrect
        Row{
            TextField{
                id: textfield
                placeholderText: "输入内容"
                height: bottomrect.height
                width: window.width * 0.8
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
            Button{
                text:"发送（或者按回车键）"
                height: bottomrect.height
                width: window.width * 0.2
                onClicked: {
                    qmlsendMessage()
                }
            }
        }
    }
    Rectangle{
        z: 88
        id: pannelbg
        width: parent.width
        anchors.bottom: inputPanel.top
        height: inputPanel.state == "visible" ? bottomDistance = 30 * window.height / 480 : 0
        color: "black"
    }
    InputPanel {
        id: inputPanel
        z: 88
        //x: -11
        y: window.height
        width: window.width
        visible: parent.visible
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
            // rect水平移动
            PropertyChanges {
                target: boyInfo
                x: parent.width * (smallScreen ? 0.50 : 0.65)
            }
        }

        transitions: Transition {
            // 数字（x坐标）动画，设置了easing的回弹效果和动画时间
            NumberAnimation {
                property: "x"
                easing.type: Easing.InOutQuart
                duration: 1000
            }
        }
    }
    Item {
        id: boyInfo
        z: 40
        width: parent.width * (smallScreen ? 0.50 : 0.35)
        height: parent.height
        x: parent.width
        Flickable {
            id: myflickable
            anchors.fill: parent
            contentWidth: parent.width
            contentHeight: parent.height +(smallScreen ? 208 : 20)
            Rectangle{
                anchors.fill: parent
                color: "#cc424242"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // state属性值为空字符串时（''）即默认状态
                        onClicked: container.state == 'right' ? container.state = '' : container.state = 'right'
                    }
                }
            }

            Image {
                id: boyID
                source: "qrc:/tcpserver/images/boy.jpg"
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
                text: qsTr("角色:TCP服务器")
                color: "white"
                font.bold: true
                font.pixelSize: 15
            }
            Text {
                id: idName
                text: qsTr("I  D:小胖子\n监听端口：888")
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
                        source: "qrc:/tcpserver/images/refresh.png"
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
                    netWorkInfo = tcpserver.getLocalIpAddress()
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
                                                  "messageBg":"qrc:/tcpserver/images/rightMessagebg.png",
                                                  "fatherMirroring":false,"childrenMirroring":false,
                                                  "headPhotoImage":"qrc:/tcpserver/images/boy.jpg",
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
                    tcpserver.disConnect()
                    connectSate = "已断开连接"
                }
            }

        }
    }
}
