import QtQuick 1.1

Rectangle {
    property int centerX: width/2
    property int centerY: height/2
    property color orderColor: "#bb3333"

    width: 21
    height: width
    radius: width/2
    color: "#345634"
    border.color: "#111111"
    border.width: 1

    Rectangle {
        width: parent.width/2
        height: width
        radius: width/2
        color: orderColor

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }
}
