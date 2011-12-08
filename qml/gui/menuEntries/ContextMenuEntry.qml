import QtQuick 1.1

Rectangle {
    property string buttonText: ""
    signal entryClicked(string entryName)
    property color backgroundColor: "#5e2c24"

    id: entryButton
    color: backgroundColor
    border.color: "#c0baba"
    width: entryText.paintedWidth
    height: entryText.paintedHeight

    Text {
        id: entryText
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#ffffff"
        text: buttonText
        font.pointSize: 11
        style: Text.Raised
        font.bold: true
        font.family: "FreeMono"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    MouseArea {
        id: moveFastMouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        hoverEnabled: true

        onEntered: parent.color = Qt.tint(backgroundColor, "#80202020");
        onExited: parent.color = backgroundColor;
        onClicked: entryClicked(buttonText);
    }
}
