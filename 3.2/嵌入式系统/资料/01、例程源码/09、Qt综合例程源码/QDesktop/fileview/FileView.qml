/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   fileview
* @brief         FileView.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-31
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick 2.3
import Qt.labs.folderlistmodel 2.1
import QtQuick.Controls.Styles 1.2
import QtQuick.Controls 1.2
import QtQuick.Controls 2.5
//import QtQuick.Controls 2.14
import fileIO 1.0
import "../helpbutton"
Item {
    visible: false
    id : fileView

    FileIO {
        id: myFile
        onError: console.log(msg)
    }

    HelpMessages {
        anchors.fill: parent
        z: 105
        appIcon: WINStyle ? "" : "qrc:/desktop/androidstyleicons/fileview.png"
        appName: "文件夹"
        appVersion: "Version 1.0.0"
        subtoolText: "1.此软件为文件夹浏览器，点击目录则进入目录，点击路径中的目录则返回到该目录。
2.点击非目录文件且文件须小于1M,则会打开该文件进行阅读，注意打开的文本的编码必须是UTF-8格式，否则会乱码."
        versionText: "1.【Version 1.0.0 2020-10-06】。\n初始版本"
    }

    onVisibleChanged: {
        formState.state == 'right' ? formState.state = '' : formState.state = 'right'
    }
    Component.onCompleted: {
        fileView.x = parent.width
    }
    Item {
        id: formState
        states: State {
            name: "right"
            PropertyChanges {
                target: fileView
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
    Rectangle {
        anchors.fill: parent
        id: myFileView
        color: "transparent"

        property string folderPathName: WINenv ? "file:///C:/Users/Administrator/Desktop/" : "file:/"
        property bool myFileViewPath: false
        property string currtentPathName
        property int pathIndexCount: 0
        Component.onCompleted: {
            pathIndex.model.insert(pathIndex.model.count, {"myPathName": WINenv ?"C:/Users/Administrator/Desktop" : " / ",
                                       "myPath": WINenv ? "file:///C:/Users/Administrator/Desktop/" : "file:/"})
            myFileView.pathIndexCount = pathIndex.count
            pathIndex.currentIndex = pathIndex.count -1
        }

        ListView {
            id: pathIndex
            anchors.top: parent.top
            anchors.topMargin: 30
            anchors.left: parent.left
            anchors.leftMargin: 0
            width: myFileView.width
            height: 40
            clip: true
            spacing: 2
            orientation : ListView.Horizontal
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                z: -1
            }
            delegate:Rectangle{
                width: pathText.contentWidth
                height: 40
                color: "transparent"
                radius: 1
                Text{
                    id: pathText
                    color: parent.ListView.isCurrentItem ? "white": "#25cfea"
                    text: myPathName
                    font.pixelSize: 20
                    anchors.centerIn: parent
                    font.bold: true
                }
                Text {
                    id: textMyPath
                    visible: false
                    text: myPath
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        myFileView.folderPathName = pathIndex.model.get(index).myPath
                        if (index < pathIndex.count - 1)
                            for(var i = index;  i < myFileView.pathIndexCount -1 ; i++ ){
                                listmodel.remove(index+1)
                            }
                        myFileView.pathIndexCount = pathIndex.count
                    }
                }
            }
            model:ListModel{
                id:listmodel
            }
        }

        function insertItem(){
            pathIndex.model.insert(pathIndex.model.count, {"myPathName":"> " + myFileView.currtentPathName, "myPath": myFileView.folderPathName})
            myFileView.pathIndexCount = pathIndex.count
            pathIndex.currentIndex = pathIndex.count -1
        }

        ListView{
            id: listFileView
            spacing: 2
            ScrollBar.vertical: ScrollBar {
                id: scrollBar
                z: 20
                width: 5
                hoverEnabled: true
                background: Rectangle {color: "#33bbbbbb"}
                onActiveChanged: {
                    active = true;
                }
                Component.onCompleted: {
                    scrollBar.active = true;
                }
                contentItem: Rectangle{
                    implicitWidth: 5
                    implicitHeight: 100
                    radius: 2
                    color: scrollBar.hovered ? "#25cfea" : "#25cfea"
                }
            }
            anchors{
                bottom: parent.bottom
                bottomMargin: 4
                right: myFileView.right
                left: myFileView.left
                top: pathIndex.bottom
            }
            clip: true
            delegate:Rectangle{
                height: 60
                width: myFileView.width
                color: "transparent"
                Image {
                    id: folderIcon
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    anchors.verticalCenter: parent.verticalCenter
                    width: 59
                    height: 48
                    opacity: 0.8
                    source: folderModel.get(index, "fileIsDir") ? "qrc:/fileview/images/folder.png"  : "qrc:/fileview/images/other_file.png.png"
                }

                Text {
                    id: textfileName
                    text: fileName
                    width: parent.width - 40
                    color: "#25cfea"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    anchors.left: folderIcon.right
                    anchors.leftMargin: 10
                }
                Text {
                    id: textModify
                    text: fileModified
                    anchors.top: textfileName.bottom
                    anchors.topMargin: 10
                    anchors.left: textfileName.left
                    color: "white"
                    font.bold: true
                    font.pixelSize: 13
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                    height: 1
                    z: -1
                    color: "#90333333"
                }
                Image {
                    id: next
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                    anchors.topMargin: 30
                    source: folderModel.get(index, "fileIsDir") ? "qrc:/fileview/images/next.png" : ""
                    width: 20
                    height: 20
                    opacity: 0.6
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        if(folderModel.isFolder(index)){
                            myFileView.folderPathName = folderModel.get(index, "fileURL")
                            myFileView.currtentPathName=folderModel.get(index, "fileName")
                            myFileView.insertItem()
                        } else {
                            var size = folderModel.get(index, "fileSize")
                            if (size < 10000) {
                                switch (folderModel.get(index, "fileSuffix")) {
                                case "txt":
                                case "sh":
                                case "conf":
                                case "cpp":
                                case "c":
                                case "h":
                                case "sh":
                                case "local":
                                case "lrc":
                                case "blacklist":
                                case "py":
                                    break
                                default:
                                    warningDialog.open()
                                    return;
                                }
                                dialog.open()
                                myFile.source = folderModel.get(index, "filePath")
                                myText.text = myFile.read()
                            } else {
                                warningDialog.open()
                            }
                        }

                    }
                }
            }
            model: FolderListModel{
                id:folderModel
                objectName: "folderModel"
                showDirs: true
                showFiles: true
                showDirsFirst: true
                showDotAndDotDot :false
                nameFilters: ["*"]
                folder:myFileView.folderPathName
                onFolderChanged: {

                }
            }
        }

        Dialog {
            id: warningDialog
            modal: true
            width: 400
            height: 100
            anchors.centerIn: parent
            //standardButtons: Dialog.Close
            background: Rectangle {anchors.fill: parent;color: "#1f1e58"}
            Text {
                anchors.centerIn: parent
                width: parent.width
                color: "white"
                font.bold: true
                font.pixelSize: 15
                text: "Warning: File is too large or abnormal!"
                wrapMode: Text.WordWrap
            }
            Button {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -10
                anchors.right: parent.right
                anchors.rightMargin: -18
                background: Rectangle {
                    color: "transparent"
                }
                width: 50
                height: 40
                Text {
                    id: closeWarningDialog
                    anchors.centerIn: parent
                    text: qsTr("关闭")
                    font.bold: true
                    color: "white"
                    font.pixelSize: 18
                }
                onClicked: {
                    warningDialog.close()
                }
            }
        }

        Dialog {
            id: dialog
            modal: true
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
            //standardButtons:  Dialog.Close
            background: Rectangle {anchors.fill: parent;color: "#1f1e58"}
            //            Rectangle {
            //                anchors.fill: parent
            //                color: "#33bbbbbb"
            //            }
            //            onClosed: {
            //                myText.text = ""
            //                dialog.close()
            //            }

            Button {
                anchors.top: parent.top
                anchors.topMargin: -10
                anchors.left: parent.left
                anchors.leftMargin: -18
                background: Rectangle {
                    color: "transparent"
                }
                width: 50
                height: 40
                Text {
                    id: back
                    anchors.centerIn: parent
                    text: qsTr("< 返回")
                    font.bold: true
                    color: "white"
                    font.pixelSize: 18
                }
                onClicked: {
                    myText.text = ""
                    dialog.close()
                }
            }

            Flickable {
                anchors.top: parent.top
                anchors.topMargin: 50
                width: parent.width
                height: parent.height
                contentWidth: parent.width
                contentHeight: myText.height
                Text {
                    id: myText
                    width: parent.width
                    wrapMode: Text.WrapAnywhere
                    color: "white"
                    font.bold: true
                    font.pixelSize: 15
                }
                ScrollBar.vertical: ScrollBar {
                    id: scrollBar2
                    z: 20
                    hoverEnabled: true
                    anchors.right: parent.right
                    anchors.rightMargin: -20
                    width: 5
                    background: Rectangle {color: "#33bbbbbb"}
                    onActiveChanged: {
                        active = true;
                    }
                    Component.onCompleted: {
                        scrollBar.active = true;
                    }
                    contentItem: Rectangle{
                        implicitWidth: 1
                        implicitHeight: 100
                        radius: 1
                        color: scrollBar.hovered ? "#25cfea" : "#25cfea"
                    }
                }
            }
        }
    }
}

