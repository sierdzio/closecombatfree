import QtQuick 1.1

Item {
    property int currentZoom: 100
    property bool open: false
    property color backgroundColor: "#dd333232"

    signal pauseEntryClicked();
    signal optionsEntryClicked();
    signal uiModeEntryClicked();
    signal zoomIn();
    signal zoomOut();

    signal toggleMenu();
    onToggleMenu: {
        open = !open;
    }

    signal preferencesEntryClicked()
    onPreferencesEntryClicked: {
        console.log("Preferences.");
    }

    signal saveGameEntryClicked()
    onSaveGameEntryClicked: {
        console.log("Save game.");
    }

    signal loadGameEntryClicked()
    onLoadGameEntryClicked: {
        console.log("Load game.");
    }

    signal quitEntryClicked()
    onQuitEntryClicked: {
        console.log("Bailing out.");
        Qt.quit();
    }

    Component.onCompleted: {
        preferencesEntry.entryClicked.connect(preferencesEntryClicked);
        saveGameEntry.entryClicked.connect(saveGameEntryClicked);
        loadGameEntry.entryClicked.connect(loadGameEntryClicked);
        quitEntry.entryClicked.connect(quitEntryClicked);
    }

    id: root
    width: 30
    state: "closed"

    Column {
        id: menu
        visible: false
        y: -height

        MenuEntry {
            id: preferencesEntry
            text: "Preferences"
            size: root.width
        }

        MenuEntry {
            id: saveGameEntry
            text: "Save game"
            size: root.width
        }

        MenuEntry {
            id: loadGameEntry
            text: "Load game"
            size: root.width
        }

        MenuEntry {
            id: quitEntry
            text: "Quit"
            size: root.width
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
                    property: "y";
                    to: 0;
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
                    property: "y";
                    to: -menu.height;
                    duration: 300
                }
                ScriptAction {
                    script: menu.visible = false;
                }
            }
        }
    ]
}
