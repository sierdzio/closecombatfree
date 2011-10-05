import QtQuick 1.1

Item {
    property int centerX: hull.width/2
    property int centerY: hull.height/2
    property color hullColor: "#7b8259"

    id: root
    width: hull.width
    height: hull.height

    Rectangle {
        id: hull
        width: 62
        height: 100
        color: hullColor
        radius: 5
        border.width: 2
        border.color: "#443512"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }
}
