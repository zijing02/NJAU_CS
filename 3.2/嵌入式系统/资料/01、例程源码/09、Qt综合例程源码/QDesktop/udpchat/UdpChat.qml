/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   udpchat
* @brief         UdpChat.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-31
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.2
import udpsender 1.0
import udpreciver 1.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick 2.12
import QtQuick.VirtualKeyboard 2.2
import "../helpbutton"

Item {
    visible: false
    id: window

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/udpchat.png"
        appName: "Udp聊天"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件为udp通信聊天，点击聊天中的人物头像可获取本机的ip信息，清空聊天记录等。
2.此可以自收自发信息，也可以使用手机上的其他udp通信软件向此软件发送信息，发送的信息会显示到屏幕上，还能知道是哪个ip发送的。
3.输入软件要发送的ip地址与端口，可以是本机的ip地址，那么它就自发自收了！如果是其他接收的ip，请使用udp相关的软件监听此端口发来的消息！"
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }

    Component.onCompleted: {
        mainListview.model.insert(mainListview.model.count,
                                  {"detail":"您好，我是UDP聊天，本程序是发送者，也是接收者，点击我头像可查看本地IP信息",
                                      "messageBg":"qrc:/udpchat/images/rightMessagebg.png",
                                      "fatherMirroring":false,"childrenMirroring":false,
                                      "headPhotoImage":"qrc:/udpchat/images/boy.jpg",
                                      "messBackground":"#b8fa99"})
        mainListview.currentIndex = mainListview.count - 1
        netWorkInfo = myUdpsender.getLocalIpAddress()
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

    property string netWorkInfo: ""
    UdpSender{
        id: myUdpsender
    }
    UdpReciver{
        id: myUdpReciver
        onRecMessageChanged: {
            // listmodel.append({"detail":myUdpsender.recMessage})
            mainListview.model.insert(mainListview.model.count,{"detail":myUdpReciver.recMessage,
                                          "messageBg":"qrc:/udpchat/images/leftMessagebg.png",
                                          "fatherMirroring":true,"childrenMirroring":true,
                                          "headPhotoImage":"qrc:/udpchat/images/gril.jpg",
                                          "messBackground":"#e3e3f8"})
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
                            wrapMode: fitWidth(label1.text) > window.width*0.2 ? Text.WrapAnywhere:Text.NoWrap
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
                                          "messageBg":"qrc:/udpchat/images/rightMessagebg.png",
                                          "fatherMirroring":false,"childrenMirroring":false,
                                          "headPhotoImage":"qrc:/udpchat/images/boy.jpg",
                                          "messBackground":"#b8fa99"})
            mainListview.currentIndex = mainListview.count - 1
            myUdpsender.send(textfield.text)
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
            Rectangle {
                height: bottomrect.height
                width: window.width * 0.35
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
                    placeholderText: "目标ip"
                    height: bottomrect.height - 5
                    anchors.verticalCenter: parent.verticalCenter
                    width: window.width * 0.15
                    anchors.left: ipText.right
                    font.pixelSize: 13
                    color: "white"
                    onFocusChanged: {
                        if(inputPanel.state == 'visible' )
                            container.state = ''
                    }
                    onTextChanged: {
                        myUdpsender.setUdpReciverInfo(ipTextfield.text,portTextfield.text);
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
                    placeholderText: "port"
                    anchors.left: port.right
                    height: bottomrect.height - 5
                    width: window.width * 0.08
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 13
                    color: "white"
                    onFocusChanged: {
                        if(inputPanel.state == 'visible' )
                            container.state = ''
                    }
                    onTextChanged: {
                        myUdpsender.setUdpReciverInfo(ipTextfield.text,portTextfield.text);
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
            }

            TextField{
                id: textfield
                font.pixelSize: 13
                placeholderText: "输入要发消息内容"
                height: bottomrect.height
                width: window.width * 0.45
                color: "white"
                Keys.enabled: true
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
                font.pixelSize: 13
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
        height: inputPanel.state == "visible" ? 28 * window.height / 480 : 0
        color: "black"
    }
    InputPanel {
        id: inputPanel
        z: 88
        //x: -11
        y: window.height
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
            contentHeight: parent.height + (smallScreen ? 208 : 20)
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
                source: "qrc:/udpchat/images/boy.jpg"
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
                text: qsTr("角色:UDP聊天")
                color: "white"
                font.bold: true
                font.pixelSize: 15
            }
            Text {
                id: idName
                text: qsTr("监听端口:666")
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
                        id: refresh
                        anchors.centerIn: parent
                        source: "qrc:/udpchat/images/refresh.png"
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
                    netWorkInfo = myUdpsender.getLocalIpAddress()
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
                    mainListview.model.insert(mainListview.model.count,
                                              {"detail":"留一个，方可获得本机IP信息",
                                                  "messageBg":"qrc:/udpchat/images/rightMessagebg.png",
                                                  "fatherMirroring":false,"childrenMirroring":false,
                                                  "headPhotoImage":"qrc:/udpchat/images/boy.jpg",
                                                  "messBackground":"#b8fa99"})
                    mainListview.currentIndex = mainListview.count - 1
                }
            }

        }
    }
}


