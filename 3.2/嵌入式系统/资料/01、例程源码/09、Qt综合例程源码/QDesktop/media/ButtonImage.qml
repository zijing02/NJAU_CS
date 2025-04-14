import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Button{
    id:root
    width: 100
    height: 30
    property string iconImage:""
    style:ButtonStyle{
        background: Rectangle{
            color: "transparent"
            Image{
                width:root.width
                height: root.height
                source: iconImage
            }
        }
    }
}

