import QtQuick 1.1

Item {
    property int centerX: tankWidth/2
    property int centerY: tankHeight/2
    property color hullColor: "#7b8259"
    property int tankWidth: 62 // mm - should be properly recalculated later on
    property int tankHeight: 100 // mm - should be properly recalculated later on

    id: root
    width: tankWidth
    height: tankHeight

    Rectangle {
        id: hull
        width: tankWidth
        height: tankHeight
        color: hullColor
        radius: 5
        border.width: 2
        border.color: "#443512"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }
}
