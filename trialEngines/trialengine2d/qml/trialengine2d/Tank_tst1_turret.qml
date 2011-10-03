import QtQuick 1.0

Rectangle {
    property int centerX: body.width/2
    property int centerY: (body.height/2) + barrel.height
    property string bodyTexture: "img/tank_tst1_turret_main.png"

    id: root
    color: "#00000000"
    width: body.width
    height: body.height + barrel.height

    Image {
        id: body
        width: 60
        height: 60
        sourceSize.height: 60
        sourceSize.width: 60
        source: bodyTexture
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    Rectangle {
        id: barrel
        width: 3
        height: 72
        color: "#7b8259"
        border.width: 1
        border.color: "#4f5e1f"
        anchors.bottom: body.top
        anchors.bottomMargin: -1
        anchors.horizontalCenter: body.horizontalCenter
    }
}
