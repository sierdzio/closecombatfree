import QtQuick 1.1

Rectangle {
    property int unitWidth: 60
    property int unitHeight: 60
    property int centerX: width/2
    property int centerY: height/2
    property int borderMargin: 16

    width: unitWidth + borderMargin
    height: unitHeight + borderMargin
    x: -(borderMargin/2)
    y: -(borderMargin/2)

    color: "#00000000"
    border.width: 2
    border.color: "#eeee11"

    Rectangle {
        width: 10
        height: 10
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: parent.centerY - (height/2)
        anchors.leftMargin: parent.centerX - (width/2)

        color: "#00000000"
        border.width: 2
        border.color: "#eeee11"
    }
}
