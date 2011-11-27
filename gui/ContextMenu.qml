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
    height: (__buttonHeight + 1) * 7 + 1
    color: "#331008"
    border.width: 2
    border.color: "#000000"
    focus: true

    ContextMenuEntry {
        id: moveFastButton
        buttonText: qsTr("Move fast")
        width: __buttonWidth
        height: __buttonHeight
        anchors.top: parent.top
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: moveFastButton.entryClicked.connect(prepareSignal);
    }
    ContextMenuEntry {
        id: moveButton
        buttonText: qsTr("Move")
        width: __buttonWidth
        height: __buttonHeight
        anchors.top: moveFastButton.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: moveButton.entryClicked.connect(prepareSignal);
    }
    ContextMenuEntry {
        id: sneakButton
        buttonText: qsTr("Sneak")
        width: __buttonWidth
        height: __buttonHeight
        anchors.top: moveButton.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: sneakButton.entryClicked.connect(prepareSignal);
    }
    ContextMenuEntry {
        id: smokeButton
        buttonText: qsTr("Smoke")
        width: __buttonWidth
        height: __buttonHeight
        anchors.top: sneakButton.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: smokeButton.entryClicked.connect(prepareSignal);
    }
    ContextMenuEntry {
        id: attackButton
        buttonText: qsTr("Attack")
        width: __buttonWidth
        height: __buttonHeight
        anchors.top: smokeButton.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: attackButton.entryClicked.connect(prepareSignal);
    }
    ContextMenuEntry {
        id: ambushButton
        buttonText: qsTr("Ambush")
        width: __buttonWidth
        height: __buttonHeight
        anchors.top: attackButton.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: ambushButton.entryClicked.connect(prepareSignal);
    }
    ContextMenuEntry {
        id: defendButton
        buttonText: qsTr("Defend")
        width: __buttonWidth
        height: __buttonHeight
        anchors.top: ambushButton.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: defendButton.entryClicked.connect(prepareSignal);
    }
}