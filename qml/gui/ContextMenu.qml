import QtQuick 1.1
import "menuEntries"

Rectangle {    
    property int buttonWidth: moveFastButton.width
    property int buttonHeight: 19
    property int unitIndex: -1
    property color backgroundColor: "#5e2c24"
    property color entryBackgroundColor: backgroundColor

    signal menuEntryClicked(int currentUnitIndex, string menuEntry)

    function prepareSignal(menuEntry) {
        menuEntryClicked(unitIndex, menuEntry);
    }

    id: root
    width: entries.width + 5
    height: entries.height + 5
    color: backgroundColor
    border.width: 2
    border.color: "#000000"
//    focus: true

    Grid {
        id: entries
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 2
        anchors.leftMargin: 2
        spacing: 1
        rows: 9
        flow: Grid.TopToBottom

        ContextMenuEntry {
            id: moveFastButton
            buttonText: qsTr("Move fast")
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: moveFastButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: moveButton
            buttonText: qsTr("Move")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: moveButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: sneakButton
            buttonText: qsTr("Sneak")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: sneakButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: smokeButton
            buttonText: qsTr("Smoke")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: smokeButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: attackButton
            buttonText: qsTr("Attack")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: attackButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: ambushButton
            buttonText: qsTr("Ambush")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: ambushButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: defendButton
            buttonText: qsTr("Defend")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: defendButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: stopButton
            buttonText: qsTr("Stop")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: stopButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: followButton
            buttonText: qsTr("Follow")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: followButton.entryClicked.connect(prepareSignal);
        }
    }
}
