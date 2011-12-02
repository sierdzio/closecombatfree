import QtQuick 1.1

Prop {
    objectName: "props/Tree_tst"
    width: top.width
    height: top.height

    Rectangle {
        id: base
        width: 4
        height: 4
        radius: 1
        color: "#4f1708"

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
