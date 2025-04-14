/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   photoview
* @brief         PhotoListView.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-16
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.2

Item {
//    function photoInit(){
//        myPhoto.add(WINenv ? qsTr("C:/Users/Administrator/Desktop/img") : qsTr("/home/root/img"))
//    }
//    Component.onCompleted:{
//        photoInit()
//    }

    // PhotoListView
    GridView  {
        id: photoList
        visible: true
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        focus: true
        interactive: true
        cellWidth: photoList.width / 4
        cellHeight: photoList.width / 4
        clip: true
        z: 1
        model: myPhoto
        delegate: Rectangle {
            id: itembg
            width: photoList.cellWidth
            height: photoList.cellWidth
            color: "transparent"
            Image {
                id: photo
                source: path
                anchors.centerIn: parent
                width: parent.height - 10
                height: parent.height - 20
                smooth: true
                fillMode: Image.PreserveAspectFit
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                id: photoName
                width: parent.width
                anchors.top: photo.bottom
                anchors.horizontalCenter: photo.horizontalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.left: parent.left
                text: title
                elide: Text.ElideRight
                color: parent.ListView.isCurrentItem ? "#27def9" : "white"
                font.bold: parent.ListView.isCurrentItem
            }
            MouseArea {
                id:mouserArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    myPhoto.currentIndex = index
                    photoSwipeView.currentIndex = 1
                    currtenPhotoPath = myPhoto.getcurrentPath()
                }
            }
        }
    }
}
