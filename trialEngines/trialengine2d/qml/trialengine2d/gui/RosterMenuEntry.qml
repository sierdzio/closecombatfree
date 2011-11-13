import QtQuick 1.1

Rectangle {
    property string entryText: ""
    property string entryLogo: ""
    property int index: 0

    signal entryClicked(string entryName)

    id: root
    color: "#5e6c24"
    border.color: "#1e1c00"
    width: 125
    height: 50

    Image {
        id: logo
        height: 50
        width: 50
        anchors.left: parent.left
        anchors.top: parent.top
        source: entryLogo
    }

    Text {
        id: entryDescription
        height: 50
        width: 75
        anchors.left: logo.right
        anchors.top: parent.top
        color: "#000000"
        text: entryText
        font.pointSize: 9
        style: Text.Raised
        font.bold: true
        font.family: "FreeMono"
        verticalAlignment: Text.Top
        horizontalAlignment: Text.AlignHCenter
    }

    MouseArea {
        id: moveFastMouseArea
        anchors.fill: parent
        hoverEnabled: true

        onEntered: parent.color = "#2e3c24";
        onExited: parent.color = "#5e6c24";
//        onClicked: mouse.accepted = false;//entryClicked(entryText);
    }
}
