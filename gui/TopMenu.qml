import QtQuick 1.1

Item {
    property real currentZoom: zoomBox.currentZoom
    property bool open: trigger.poppedUp
    property color backgroundColor: "#dd333232"

    signal pauseEntryClicked();
    signal optionsEntryClicked();
    signal uiModeEntryClicked();

    onOptionsEntryClicked: {
        console.log("Options entry clicked.");
    }

    onUiModeEntryClicked: {
        toggleUiMode();
    }

    Component.onCompleted: {
        pauseEntry.entryClicked.connect(pauseEntryClicked);
        optionsEntry.entryClicked.connect(optionsEntryClicked);
        modeEntry.entryClicked.connect(uiModeEntryClicked);
    }

    id: root
    height: 30

    PopUpArrow {
        id: trigger
        anchors.top: parent.top
        anchors.right: parent.right
        size: root.height
    }

    Row {
        id: menu
        visible: false
        anchors.right: trigger.left
        anchors.top: trigger.top

        ZoomBox {
            id: zoomBox
            size: root.height
        }

        MenuEntry {
            id: pauseEntry
            text: "Pause"
            size: root.height
        }

        MenuEntry {
            id: optionsEntry
            text: "Options"
            size: root.height
        }

        MenuEntry {
            id: modeEntry
            text: "UI Mode"
            additionalText: uiMode
            size: root.height
        }
    }

    states: [
        State {
            when: (open == true)
            name: "opened"
            PropertyChanges {
                target: menu
                visible: true
            }
        }
    ]
}
