/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   photoview
* @brief         DisplayPhoto.qml
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-16
*******************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQml 2.12
Item {
    id: displayView
    visible: false
    onVisibleChanged: pathView.currentIndex = myPhoto.currentIndex

    Item {
        id: topMenu
        y: -50
        x: 0
        z: 50
        height: 50
        width: parent.width
        Rectangle{
            id: topMenuBg
            anchors.fill: parent
            color: "#99111111"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    topContainer.state == 'topShow' ? topContainer.state = '' : topContainer.state = 'topShow'
                    bottomContainer.state == 'bottomShow' ? bottomContainer.state = '' : bottomContainer.state = 'bottomShow'
                }
            }
            Button {
                id: btnBack
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                style: ButtonStyle {
                    background: Image {
                        anchors.fill: parent
                        source: "qrc:/photoview/icons/back.png"
                    }
                }
                onClicked: photoSwipeView.currentIndex = 0
            }
        }
    }
    Item {
        id: topContainer
        states: State {
            name: "topShow"
            PropertyChanges {
                target: topMenu
                y: 0
            }
        }
        transitions: Transition {
            NumberAnimation {
                property: "y"
                easing.type: Easing.InOutQuart
                duration: 500
            }
        }
    }

    Item {
        id: bottomMenu
        y: parent.height
        x: 0
        z: 20
        height: 50
        width: parent.width
        Rectangle{
            id: bottomMenuBg
            anchors.fill: parent
            color: "#99111111"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    topContainer.state == 'topShow' ? topContainer.state = '' : topContainer.state = 'topShow'
                    bottomContainer.state == 'bottomShow' ? bottomContainer.state = '' : bottomContainer.state = 'bottomShow'
                }
            }
        }
    }
    Item {
        id: bottomContainer
        states: State {
            name: "bottomShow"
            PropertyChanges {
                target: bottomMenu
                y: displayView.height - 50
            }
        }
        transitions: Transition {
            NumberAnimation {
                property: "y"
                easing.type: Easing.InOutQuart
                duration: 500
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        id:coverflow
        color:"black"
        Component.onCompleted: {
            //console.log(coverflow.width,coverflow,height)
        }

        property ListModel model
        property int itemCount: 3
        PathView{
            id: pathView
            model:myPhoto
            anchors.centerIn: parent
            delegate: Item {
                id:delegateItem
                width: parent.width
                height: parent.height
                z:PathView.iconZ
                scale:PathView.iconScale
                Image{
                    id:image
                    source: path
                    width: delegateItem.width
                    height: delegateItem.height
                    smooth: true
                    fillMode: Image.PreserveAspectFit
                    opacity: pathView.currentIndex == index ? 1 : 0.1
                    enabled: pathView.currentIndex == index ? true : false
                    antialiasing: true
                    MouseArea {
                        id: mapDragArea
                        anchors.fill: image
                        onClicked: {
                            topContainer.state == 'topShow' ? topContainer.state = '' : topContainer.state = 'topShow'
                            bottomContainer.state == 'bottomShow' ? bottomContainer.state = '' : bottomContainer.state = 'bottomShow'
                        }
                        onPressAndHold: drag.target = image
                        onWheel: {
                            var datla = wheel.angleDelta.y/120
                            if(datla > 0){
                                image.scale = image.scale/0.9
                            }
                            else{
                                image.scale = image.scale*0.9
                            }
                        }
                    }
                }
                transform: Rotation{
                    origin.x:image.width/2.0
                    origin.y:image.height/2.0
                    axis{x:0;y:1;z:0}
                    angle:delegateItem.PathView.iconAngle
                }
            }
            path:coverFlowPath
            pathItemCount: coverflow.itemCount
            anchors.fill: parent

            preferredHighlightBegin: 0.5
            preferredHighlightEnd: 0.5

        }

        Path{
            id:coverFlowPath
            startX: 0
            startY: coverflow.height/2

            PathAttribute{name:"iconZ";value: 0}
            PathAttribute{name:"iconAngle";value: 70}
            PathAttribute{name:"iconScale";value: 0.1}
            PathLine{x:coverflow.width/2;y:coverflow.height/2}

            PathAttribute{name:"iconZ";value: 100}
            PathAttribute{name:"iconAngle";value: 0}
            PathAttribute{name:"iconScale";value: 1.0}

            PathLine{x:coverflow.width;y:coverflow.height/2}
            PathAttribute{name:"iconZ";value: 0}
            PathAttribute{name:"iconAngle";value: -70}
            PathAttribute{name:"iconScale";value: 0.1}
            PathPercent{value:1.0}

        }
    }
}
