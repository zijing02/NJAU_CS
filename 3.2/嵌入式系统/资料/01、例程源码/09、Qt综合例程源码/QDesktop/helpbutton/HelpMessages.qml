/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   helpbutton
* @brief         HelpMessages.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-31
*******************************************************************/
import QtQuick 2.0
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
Item {
    property string subtoolText
    property string versionText
    property bool showFlag: false
    property string appIcon
    property string appVersion
    property string appName
    Rectangle {
        anchors.fill: parent
        color: showFlag ? "#1f1e58" : "transparent"
        StackView {
            id: stack
            anchors.centerIn: parent
            anchors.fill: parent
            property var home: null

            Item {
                anchors.top: parent.top
                anchors.topMargin: 0
//                anchors.leftMargin: 5
//                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 5
                width: 50
                height: 30
                Text {
                    id: about
                    anchors.centerIn: parent
                    text: qsTr("关于")
                    font.pixelSize: 18
                    color: "white"
                    font.bold: true
                    opacity: 0.5
                    visible: stack.depth == 0
                }
//                Image {
//                    id: about
//                    source: "qrc:/helpbutton/images/about.png"
//                    opacity: 0.5
//                    visible: stack.depth == 0
//                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: about.opacity = 1
                    onExited: about.opacity = 0.5
                    onClicked: {
                        if(stack.depth == 0)
                            stack.push(page1)
                        showFlag = true
                    }
                }
            }
        }

        Component {
            id: page1

            Rectangle {
                color: "#1f1e58"
                Flickable {
                    anchors.fill: parent
                    contentWidth: parent.width
                    contentHeight:  parent.height + (smallScreen ? 208 : 20)
                    Text {
                        id: backHome
                        anchors.top: parent.top
                        anchors.topMargin: 5
                        anchors.leftMargin: 5
                        anchors.left: parent.left
                        width: parent.width
                        height: 50
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("< 返回")
                        font.bold: true
                        color: "white"
                        font.pixelSize: 18
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                showFlag = false
                                stack.clear()
                            }
                        }
                    }
                    Image {
                        id: appImage
                        source: appIcon
                        width: 100
                        height: 100
                        anchors.bottom: listview.top
                        anchors.bottomMargin: 100
                        anchors.horizontalCenter: listview.horizontalCenter
                    }

                    Text {
                        id: applicationName
                        text: appName
                        anchors.top: appImage.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "white"
                        font.bold: true
                        font.pixelSize: 25
                    }
                    Text {
                        id: applicationVersion
                        text: appVersion
                        anchors.top: applicationName.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "#cceeeeee"
                        font.pixelSize: 18
                    }
                    Text {
                        id: compny
                        text: "广州星翼 著作所有"
                        anchors.top: listview.bottom
                        anchors.topMargin: 50
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "white"
                        font.pixelSize: 18
                    }
                    Text {
                        id: copyright
                        text: "copyright © 2020-2030 Xingyi.\nAll Rights Reserved."
                        anchors.top: compny.bottom
                        anchors.topMargin: 10
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "gray"
                        font.pixelSize: 12
                    }
                    ListView {
                        id: listview
                        width: parent.width / 2
                        height: 100
                        anchors.top: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        model: ListModel {
                            ListElement {
                                title: "功能介绍"
                            }
                            ListElement {
                                title: "版本说明"
                            }

                        }
                        delegate:Rectangle {
                            height: 50
                            width: parent.width
                            color: "transparent"
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    listview.currentIndex = index
                                    listview.currentIndex  == 0 ?  stack.push(page2) : ""
                                    listview.currentIndex == 1 ?  stack.push(page3) : ""
                                }
                            }
                            Text {
                                id: functionText
                                text: title
                                color: "white"
                                font.pixelSize: 15
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Text {
                                id: go
                                font.pixelSize: 15
                                text: ">"
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                color: "white"
                            }

                            Rectangle {
                                anchors.top: parent.top
                                anchors.left: parent.left
                                width: parent.width
                                height:1
                                visible: index != 1
                                color: "gray"
                            }
                            Rectangle {
                                anchors.bottom: parent.bottom
                                anchors.left: parent.left
                                width: parent.width
                                height:1
                                color: "gray"
                            }
                        }
                    }
                }
            }
        }

        Component {
            id: page2
            Rectangle {
                color: "#1f1e58"
                Flickable {
                    anchors.fill: parent
                    contentWidth: parent.width
                    contentHeight: parent.height +20
                    Text {
                        id: subtool
                        anchors.top: parent.top
                        anchors.topMargin: 5
                        anchors.leftMargin: 5
                        anchors.left: parent.left
                        width: parent.width
                        height: 50
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("< 功能简介")
                        font.bold: true
                        color: "white"
                        font.pixelSize: 18
                        MouseArea{
                            anchors.fill: parent
                            onClicked: stack.push(page1)
                        }
                    }
                    Text {
                        anchors.top: subtool.bottom
                        anchors.topMargin: 20
                        anchors.left: parent.left
                        width: parent.width
                        font.pixelSize: 15
                        color: "#eeeeeeee"
                        text: subtoolText
                        wrapMode: Text.WrapAnywhere
                    }
                }
            }
        }

        Component {
            id: page3
            Rectangle {
                color: "#1f1e58"
                Flickable {
                    anchors.fill: parent
                    contentWidth: parent.width
                    contentHeight: parent.height +20
                    Text {
                        id: version
                        anchors.top: parent.top
                        anchors.topMargin: 5
                        anchors.leftMargin: 5
                        anchors.left: parent.left
                        width: parent.width
                        height: 50
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("< 版本说明")
                        font.bold: true
                        color: "white"
                        font.pixelSize: 18
                        MouseArea{
                            anchors.fill: parent
                            onClicked: stack.push(page1)
                        }
                    }
                    Text {
                        anchors.top: version.bottom
                        anchors.topMargin: 20
                        anchors.left: parent.left
                        width: parent.width
                        font.pixelSize: 15
                        color: "#eeeeeeee"
                        text: versionText
                    }
                }
            }
        }
    }
}
