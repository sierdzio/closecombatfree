import QtQuick 1.1

Rectangle {
    property string header: "Header"
    property string text: "Some description"

    id: root
    width: 255
    height: 42
    color: "#bb333232"
    radius: 10

    Text {
        text: header
        color: "#d6d6a1"
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 14

        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            text: root.text
            color: "#ffffff"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 9

            anchors.top: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
