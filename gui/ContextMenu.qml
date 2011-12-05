import QtQuick 1.1
import "menuEntries"

Rectangle {    
    property int __buttonWidth: moveFastButton.width
    property int __buttonHeight: 19
    property int unitIndex: -1

    signal menuEntryClicked(int currentUnitIndex, string menuEntry)

    function prepareSignal(menuEntry) {
        menuEntryClicked(unitIndex, menuEntry);
    }

    id: root
    width: entries.width + 5
    height: entries.height + 5
    color: "#331008"
    border.width: 2
    border.color: "#000000"
    focus: true

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
            height: __buttonHeight

            Component.onCompleted: moveFastButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: moveButton
            buttonText: qsTr("Move")
            width: __buttonWidth
            height: __buttonHeight

            Component.onCompleted: moveButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: sneakButton
            buttonText: qsTr("Sneak")
            width: __buttonWidth
            height: __buttonHeight

            Component.onCompleted: sneakButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: smokeButton
            buttonText: qsTr("Smoke")
            width: __buttonWidth
            height: __buttonHeight

            Component.onCompleted: smokeButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: attackButton
            buttonText: qsTr("Attack")
            width: __buttonWidth
            height: __buttonHeight

            Component.onCompleted: attackButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: ambushButton
            buttonText: qsTr("Ambush")
            width: __buttonWidth
            height: __buttonHeight

            Component.onCompleted: ambushButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: defendButton
            buttonText: qsTr("Defend")
            width: __buttonWidth
            height: __buttonHeight

            Component.onCompleted: defendButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: stopButton
            buttonText: qsTr("Stop")
            width: __buttonWidth
            height: __buttonHeight

            Component.onCompleted: stopButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: followButton
            buttonText: qsTr("Follow")
            width: __buttonWidth
            height: __buttonHeight

            Component.onCompleted: followButton.entryClicked.connect(prepareSignal);
        }
    }
}
