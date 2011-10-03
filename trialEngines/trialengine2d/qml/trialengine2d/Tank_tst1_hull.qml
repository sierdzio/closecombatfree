import QtQuick 1.0

Rectangle {
    property int centerX: hull.width/2
    property int centerY: hull.height/2

    id: root
    color: "#00000000"
    width: hull.width
    height: hull.height

    Rectangle {
        id: hull
        width: 62
        height: 100
        color: "#7b8259"
        radius: 5
        border.width: 2
        border.color: "#443512"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }
}
