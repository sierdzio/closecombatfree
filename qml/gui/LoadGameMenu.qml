import QtQuick 1.1
import "qrc:/skin/menuEntries"
import "qrc:/skin"

Rectangle {
    property color backgroundColor: "#bb333232"

    id: root
    color: backgroundColor
    border.color: Qt.tint(backgroundColor, "#88aaaaaa")
    border.width: 1

    width: configWindowWidth
    height: configWindowHeight
    radius: 20

    Component.onCompleted: {
        cancelButton.entryClicked.connect(toggleVisibility);
        entries.entryClicked.connect(gameEntryClicked);
    }

    function toggleVisibility() {
        root.visible = !root.visible;
    }

    signal gameEntryClicked (string gamePath)
    onGameEntryClicked: {
        toggleVisibility();
    }

    MouseArea {
        id: blockingArea
        anchors.fill: parent
    }

    LoadGameEntries {
        id: entries
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: buttons.top
        anchors.margins: parent.radius
    }

    // Bottom buttons
    Row {
        id: buttons

        spacing: 5
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: parent.radius
        anchors.rightMargin: parent.radius

        MenuEntry {
            id: cancelButton
            text: "Cancel"
        }
    }
}
