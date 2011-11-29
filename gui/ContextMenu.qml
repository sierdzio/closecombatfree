import QtQuick 1.1

Rectangle {    
    property int __buttonWidth: 86
    property int __buttonHeight: 19
    property int unitIndex: -1

    signal menuEntryClicked(int currentUnitIndex, string menuEntry)

    function prepareSignal(menuEntry) {
        menuEntryClicked(unitIndex, menuEntry);
    }

    id: root
    width: 90
    height: (__buttonHeight + 1) * entries.rows + 4
    color: "#331008"
    border.width: 2
    border.color: "#000000"
    focus: true

    Grid {
        id: entries
        anchors.fill: parent
        anchors.topMargin: 2
        anchors.leftMargin: 2
        spacing: 1
        rows: 8
        flow: Grid.TopToBottom

        ContextMenuEntry {
            id: moveFastButton
            buttonText: qsTr("Move fast")
            width: __buttonWidth
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
    }
}
