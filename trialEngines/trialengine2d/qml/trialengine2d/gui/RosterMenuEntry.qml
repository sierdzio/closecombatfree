import QtQuick 1.1
import "engineRosterLogic.js" as RosterLogic

Rectangle {
    property string entryText: ""
    property string entryStatusText: ""
    property color entryStatusColor: "#000000"
    property string entryLogo: ""
    property int index: 0

//    signal entryClicked(string entryName)

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
        height: 25
        width: 75
        anchors.left: logo.right
        anchors.top: parent.top
        anchors.right: parent.right
        color: "#000000"
        text: entryText
        font.pointSize: 9
        font.bold: true
        font.family: "Ubuntu Condensed"
        verticalAlignment: Text.Top
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: entryStatus
        height: 25
        width: 75
        anchors.left: logo.right
        anchors.top: entryDescription.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: entryStatusColor
        text: entryStatusText
        font.pointSize: 14
        style: Text.Raised
        font.bold: true
        font.family: "Ubuntu Condensed"
        verticalAlignment: Text.Top
        horizontalAlignment: Text.AlignHCenter
    }

    MouseArea {
        id: moveFastMouseArea
        anchors.fill: parent
        hoverEnabled: true

        onEntered: parent.color = "#2e3c24";
        onExited: parent.color = "#5e6c24";
    }

    function changeStatus(newStatusMessage) {
        entryStatusText = newStatusMessage;

        var newColor = RosterLogic.colorForStatus(newStatusMessage);
        if (newColor != "ERROR")
            entryStatusColor = newColor;
        else
            console.log("Error while changing status message color!");
    }
}
