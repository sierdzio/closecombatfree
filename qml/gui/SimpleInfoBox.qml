import QtQuick 1.1

Rectangle {
    property string headerText: "Header"
    property string bodyText: "Some description"

    id: root
    width: ((header.paintedWidth < body.paintedWidth)? body.paintedWidth : (header.paintedWidth) + 6);
    height: header.paintedHeight + body.paintedHeight + 4;
    color: "#bb333232"
    radius: 10

    Text {
        id: header
        text: headerText
        color: "#d6d6a1"
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 14

        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: body
            text: bodyText
            color: "#ffffff"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 9

            anchors.top: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
