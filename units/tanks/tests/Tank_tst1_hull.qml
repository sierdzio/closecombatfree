import QtQuick 1.1
import "../"

Hull {
    Rectangle {
        id: hull
        width: hullWidth
        height: hullHeight
        color: hullColor
        radius: 5
        border.width: 2
        border.color: "#443512"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }
}
