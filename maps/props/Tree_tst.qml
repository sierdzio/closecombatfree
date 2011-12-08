import QtQuick 1.1
import "../../qml/maps/props"

Prop {
    objectName: "tree"
    cover: "average"
    width: top.width
    height: top.height

    Rectangle {
        id: base
        width: 4
        height: 4
        radius: 1
        color: "#4f1708"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: top.width/2
        anchors.leftMargin: top.width/2

        Image {
            id: top
            visible: topVisible
            source: PWD + "img/maps/props/Tree1.png"
            width: 25
            height: width
            anchors.horizontalCenter: base.horizontalCenter
            anchors.verticalCenter: base.verticalCenter
        }
    }
}
