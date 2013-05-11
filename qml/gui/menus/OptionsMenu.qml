/****************************************************************************
** Close Combat Free is a free, easily moddable CC-like game.
** Copyright (C) 2011 Tomasz Siekierda
** Email: sierdzio@gmail.com, website: http://www.sierdzio.com
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program (in doc/ directory).
** If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
****************************************************************************/

import QtQuick 2.0
import "../../../qml/gui/menuEntries"

Item {
    property int currentZoom: 100
    property bool open: false
    property color backgroundColor: "#dd333232"
    property int size: preferencesEntry.width //30

    signal pauseEntryClicked();
    signal uiModeEntryClicked();
    signal zoomIn();
    signal zoomOut();

    signal toggleMenu();
    onToggleMenu: {
        open = !open;
    }

    signal preferencesEntryClicked();
    signal saveGameEntryClicked();
    signal loadGameEntryClicked();
    signal aboutEntryClicked();
    signal closeEntryClicked();
    signal quitEntryClicked();
    onQuitEntryClicked: {
        Qt.quit();
    }

    Component.onCompleted: {
        preferencesEntry.entryClicked.connect(preferencesEntryClicked);
        saveGameEntry.entryClicked.connect(saveGameEntryClicked);
        loadGameEntry.entryClicked.connect(loadGameEntryClicked);
        aboutEntry.entryClicked.connect(aboutEntryClicked);
        closeEntry.entryClicked.connect(closeEntryClicked);
        quitEntry.entryClicked.connect(quitEntryClicked);
    }

    id: root
    width: size
    state: "closed"

    Column {
        id: menu
        visible: false
        y: -height

        MenuEntry {
            id: preferencesEntry
            text: "Preferences"
            height: getContentsHeight()
        }

        MenuEntry {
            id: saveGameEntry
            text: "Save game"
            width: root.size
            height: getContentsHeight()
        }

        MenuEntry {
            id: loadGameEntry
            text: "Load game"
            width: root.size
            height: getContentsHeight()
        }

        MenuEntry {
            id: aboutEntry
            text: "About"
            width: root.size
            height: getContentsHeight()
        }

        MenuEntry {
            id: closeEntry
            text: "Close"
            width: root.size
            height: getContentsHeight()
        }

        MenuEntry {
            id: quitEntry
            text: "Quit"
            width: root.size
            height: getContentsHeight()
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
