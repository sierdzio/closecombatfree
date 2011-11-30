import QtQuick 1.1

Item {
    property int currentZoom: 100
    property bool open: trigger.poppedUp
    property color backgroundColor: "#dd333232"

    signal pauseEntryClicked();
    signal optionsEntryClicked();
    signal uiModeEntryClicked();
    signal zoomIn();
    signal zoomOut();

    signal openMenu();
    onOpenMenu: trigger.poppedUp = true;

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
        zoomBox.zoomIn.connect(zoomIn);
        zoomBox.zoomOut.connect(zoomOut);
    }

    id: root
    height: 30
    state: "closed"

    PopUpArrow {
        id: trigger
        anchors.top: parent.top
        anchors.right: parent.right
        size: root.height
    }

    Row {
        id: menu
        visible: false
        x: trigger.x
        anchors.top: trigger.top
        z: trigger.z - 1

        ZoomBox {
            id: zoomBox
            size: root.height
            currentZoom: root.currentZoom
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
            when: (open == false)
            name: "closed"
        },

        State {
            when: (open == true)
            name: "opened"
        }
    ]

    transitions: [
        Transition {
            from: "closed"
            to: "opened"

            SequentialAnimation {
                ScriptAction {
                    script: menu.visible = true;
                }
                NumberAnimation {
                    target: menu;
                    property: "x";
                    to: (trigger.x - menu.width);
                    duration: 300
                }
            }
        },

        Transition {
            from: "opened"
            to: "closed"

            SequentialAnimation {
                NumberAnimation {
                    target: menu;
                    property: "x";
                    to: (trigger.x);
                    duration: 300
                }
                ScriptAction {
                    script: menu.visible = false;
                }
            }
        }
    ]
}
