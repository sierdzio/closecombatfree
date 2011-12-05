import QtQuick 1.1
import "menuEntries"

Item {
    property int currentZoom: 100
    property bool open: trigger.poppedUp
    property color backgroundColor: "#dd333232"
    property int popupSize: 30

    signal pauseEntryClicked();
    signal optionsEntryClicked();
    signal uiModeEntryClicked();
    signal zoomIn();
    signal zoomOut();

    signal openMenu();
    onOpenMenu: trigger.poppedUp = true;

    onOptionsEntryClicked: {
        optionsMenu.toggleMenu();
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
    height: popupSize
    state: "closed"

    PopUpArrow {
        id: trigger
        anchors.top: parent.top
        anchors.right: parent.right
        size: popupSize
    }

    Row {
        id: menu
        visible: false
        x: trigger.x
        anchors.top: trigger.top
        z: trigger.z - 1

        ZoomBox {
            id: zoomBox
            size: popupSize
            currentZoom: root.currentZoom
        }

        MenuEntry {
            id: pauseEntry
            text: "Pause"
            height: popupSize
        }

        MenuEntry {
            id: optionsEntry
            text: "Options"
            height: popupSize

            OptionsMenu {
                id: optionsMenu
                anchors.top: optionsEntry.bottom
                anchors.left: optionsEntry.left
                z: optionsEntry.z - 1
            }
        }

        MenuEntry {
            id: modeEntry
            text: "UI Mode"
            additionalText: uiMode
            size: popupSize
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
