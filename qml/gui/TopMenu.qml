import QtQuick 1.1
import "qrc:/skin/menuEntries"

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
    signal terrainInfoEntryClicked();
    onTerrainInfoEntryClicked: {
        toggleTerrainInfoMode();
        terrainInfoEntry.additionalText = terrainInfoMode;
    }

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
        terrainInfoEntry.entryClicked.connect(terrainInfoEntryClicked);
        zoomBox.zoomIn.connect(zoomIn);
        zoomBox.zoomOut.connect(zoomOut);
        configWindowWidthChanged.connect(updateWidth);
    }

    id: root
    height: getContentsHeight()

    function getContentsHeight() {
        if (modeEntry.getContentsHeight() < popupSize)
            return popupSize;
        else
            return modeEntry.getContentsHeight();
    }

    function updateWidth() {
        if (configWindowWidth < (menu.contentWidth + trigger.width)) {
            menu.width = configWindowWidth - trigger.width;
            return;
        } else {
            menu.width = menu.contentWidth;
        }
    }

    state: "closed"

    PopUpArrow {
        id: trigger
        anchors.top: parent.top
        anchors.right: parent.right
        size: root.height
    }

    Flickable {
        id: menu
        visible: false
        anchors.top: trigger.top
        anchors.left: trigger.left
        z: trigger.z - 1

        pressDelay: 800
        height: row.height
        width: row.width
        contentHeight: row.height
        contentWidth: row.width

        Row {
            id: row

            ZoomBox {
                id: zoomBox
                size: root.height
                currentZoom: root.currentZoom
            }

            MenuEntry {
                id: optionsEntry
                text: "Options"
                height: root.height

                OptionsMenu {
                    id: optionsMenu
                    anchors.top: optionsEntry.bottom
                    anchors.left: optionsEntry.left
                    z: optionsEntry.z - 1
                }
            }

            MenuEntry {
                id: pauseEntry
                text: "Pause"
                height: root.height
            }

            MenuEntry {
                id: terrainInfoEntry
                text: "Terrain info mode"
                additionalText: "OFF"
                height: root.height
                opacity: (uiMode == "MOBILE")? 1: 0;

                Behavior on opacity {
                    NumberAnimation {}
                }
            }

            MenuEntry {
                id: modeEntry
                text: "UI Mode"
                additionalText: uiMode
                height: root.height
            }
        }

    }

    states: [
        State {
            when: (open == false)
            name: "closed"
            AnchorChanges {
                target: menu
                anchors.left: trigger.left
                anchors.right: undefined
            }
        },

        State {
            when: (open == true)
            name: "opened"
            AnchorChanges {
                target: menu
                anchors.right: trigger.left
                anchors.left: undefined
            }
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
                AnchorAnimation {
                    duration: 300
                }
            }
        },

        Transition {
            from: "opened"
            to: "closed"

            SequentialAnimation {
                AnchorAnimation {
                    duration: 300
                }
                ScriptAction {
                    script: menu.visible = false;
                }
            }
        }
    ]
}
