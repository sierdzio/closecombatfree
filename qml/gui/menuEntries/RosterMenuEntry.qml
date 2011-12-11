import QtQuick 1.1
import "qrc:/core/engineLogicHelpers.js" as Logic

Rectangle {
    property string entryText: ""
    property string entryStatusText: ""
    property color entryStatusColor: "#000000"
    property string entryLogo: ""
    property color backgroundColor: "#7e8c24"
    property bool mouseAreaEnabled: true
    property bool selected: false
    property int index: 0

    id: root
    color: backgroundColor
    border.color: "#1e1c00"
    border.width: 1
//    width: 175
//    height: 54
    width: {
        var textWidth = entryStatus.paintedWidth;

        if (entryDescription.paintedWidth > textWidth) {
            textWidth = entryDescription.paintedWidth;
        }

        return logo.width + textWidth + 5;
    }

    height: {
        if ((entryDescription.paintedHeight + entryStatus.paintedHeight) > (logo.height + 5)) {
            return (entryDescription.paintedHeight + entryStatus.paintedHeight);
        } else {
            return logo.height + 3;
        }
    }

    onEntryStatusTextChanged: {
        if (entryStatusText == "") {
            return;
        }

        var newColor = Logic.colorForStatus(entryStatusText);
        if (newColor != "ERROR")
            entryStatusColor = newColor;
        else
            console.log("Error while changing status message color!");
    }

    onSelectedChanged: {
        if (selected == true) {
            root.border.color = "#eeee11";;
        } else if (selected == false) {
            root.border.color = "#1e1c00";
        }
    }

    Image {
        id: logo
        height: 50
        width: 50
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 2
        anchors.topMargin: 2
        source: entryLogo
    }

    Text {
        id: entryDescription
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
        anchors.left: logo.right
        anchors.top: entryDescription.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: entryStatusColor
        text: entryStatusText
        font.pointSize: 12
        style: Text.Raised
        font.bold: true
        font.family: "Ubuntu Condensed"
        verticalAlignment: Text.Top
        horizontalAlignment: Text.AlignHCenter
    }

    MouseArea {
        id: moveFastMouseArea
        enabled: mouseAreaEnabled
        anchors.fill: parent
        hoverEnabled: mouseAreaEnabled

        onEntered: parent.color = Qt.tint(backgroundColor, "#80202020");
        onExited: parent.color = backgroundColor;
    }
}
