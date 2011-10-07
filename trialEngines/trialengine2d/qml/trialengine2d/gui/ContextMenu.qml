import QtQuick 1.1

Rectangle {    
    property int __buttonWidth: 86
    property int __buttonHeight: 19

    signal menuEntryClicked(string menuEntry)
//    onMenuEntryClicked: { console.log("Signal fired inside ContextMenu. Name: " + menuEntry); }

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

        Component.onCompleted: moveFastButton.entryClicked.connect(menuEntryClicked);
    }
    ContextMenuEntry {
        id: moveButton
        buttonText: qsTr("Move")
        width: __buttonWidth
        height: __buttonHeight
        anchors.top: moveFastButton.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: moveButton.entryClicked.connect(menuEntryClicked);
    }
    ContextMenuEntry {
        id: sneakButton
        buttonText: qsTr("Sneak")
        width: __buttonWidth
        height: __buttonHeight
        anchors.top: moveButton.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: sneakButton.entryClicked.connect(menuEntryClicked);
    }
    ContextMenuEntry {
        id: smokeButton
        buttonText: qsTr("Smoke")
        width: __buttonWidth
        height: __buttonHeight
        anchors.top: sneakButton.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: smokeButton.entryClicked.connect(menuEntryClicked);
    }
    ContextMenuEntry {
        id: attackButton
        buttonText: qsTr("Attack")
        width: __buttonWidth
        height: __buttonHeight
        anchors.top: smokeButton.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: attackButton.entryClicked.connect(menuEntryClicked);
    }
    ContextMenuEntry {
        id: ambushButton
        buttonText: qsTr("Ambush")
        width: __buttonWidth
        height: __buttonHeight
        anchors.top: attackButton.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: ambushButton.entryClicked.connect(menuEntryClicked);
    }
    ContextMenuEntry {
        id: defendButton
        buttonText: qsTr("Defend")
        width: __buttonWidth
        height: __buttonHeight
        anchors.top: ambushButton.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter

        Component.onCompleted: defendButton.entryClicked.connect(menuEntryClicked);
    }
}
