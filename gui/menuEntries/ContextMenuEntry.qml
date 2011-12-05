import QtQuick 1.1

Rectangle {
    property string buttonText: ""
    signal entryClicked(string entryName)

    id: entryButton
    color: "#5e2c24"
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

        onEntered: parent.color = "#7e4c44";
        onExited: parent.color = "#5e2c24";
        onClicked: entryClicked(buttonText);
    }
}
