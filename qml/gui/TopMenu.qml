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

import QtQuick 1.1
import "../../qml/gui/menuEntries"

Item {
    property int currentZoom: 100
    property bool open: trigger.poppedUp
    property color backgroundColor: "#dd333232"
    property int popupSize: 30

    signal pauseEntryClicked();
    signal optionsEntryClicked();
    signal zoomIn();
    signal zoomOut();
    signal terrainInfoEntryClicked();
    signal closeScenario();
    signal preferences();
    signal save();
    signal load();

    onTerrainInfoEntryClicked: {
        Config.toggleTerrainInfoMode();
        terrainInfoEntry.additionalText = Config.terrainInfoMode;
    }

    signal openMenu();
    onOpenMenu: trigger.poppedUp = true;

    signal toggleMenu();
    onToggleMenu: trigger.poppedUp = !trigger.poppedUp;

    onOptionsEntryClicked: {
        optionsMenu.toggleMenu();
    }

    Component.onCompleted: {
        pauseEntry.entryClicked.connect(pauseEntryClicked);
        optionsEntry.entryClicked.connect(optionsEntryClicked);
        optionsMenu.preferencesEntryClicked.connect(preferences);
        optionsMenu.closeEntryClicked.connect(closeScenario);
        optionsMenu.saveGameEntryClicked.connect(save);
        optionsMenu.loadGameEntryClicked.connect(load);
        terrainInfoEntry.entryClicked.connect(terrainInfoEntryClicked);
        zoomBox.zoomIn.connect(zoomIn);
        zoomBox.zoomOut.connect(zoomOut);
        Config.windowWidthChanged.connect(updateWidth);
    }

    id: root
    height: getContentsHeight()

    function getContentsHeight() {
        if (terrainInfoEntry.getContentsHeight() < popupSize)
            return popupSize;
        else
            return terrainInfoEntry.getContentsHeight();
    }

    function updateWidth() {
        if (Config.windowWidth < (menu.contentWidth + trigger.width)) {
            menu.width = Config.windowWidth - trigger.width;
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

            MenuEntry {
                id: terrainInfoEntry
                text: "Terrain info mode"
                additionalText: "OFF"
                height: root.height
                // This is a serious and nasty hack, and should be
                // redesigned.
                opacity: (Config.uiMode == "MOBILE")? 1 : 0.001;
                enabled: (Config.uiMode == "MOBILE")? true : false;

                Behavior on opacity {
                    NumberAnimation {}
                }
            }

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
                    script: {
                        menu.visible = true;
                    }
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
                    script: {
                        menu.visible = false;
                    }
                }
            }
        }
    ]
}
