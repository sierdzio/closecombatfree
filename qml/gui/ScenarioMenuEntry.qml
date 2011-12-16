import QtQuick 1.1

Rectangle {
    property string text: ""
    signal entryClicked (string text)

    width: entryText.paintedWidth + 10
    height: entryText.paintedHeight + 10
    color: "#000000"
    border.color: "#a0a0a0"
    border.width: 1

    Text {
        id: entryText
        color: "#ffffff"
        text: parent.text
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 10
        font.bold: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            entryClicked(parent.text);
        }
    }
}
