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

import QtQuick 2.1
import "../../../qml/gui/menuEntries"

Rectangle {    
    property int buttonWidth: moveFastButton.width
    property int buttonHeight: 19
    property color backgroundColor: "#5e2c24"
    property color entryBackgroundColor: backgroundColor

    signal menuEntryClicked(string menuEntry)

    function prepareSignal(menuEntry) {
        menuEntryClicked(menuEntry);
    }

    id: root
    width: entries.width + 5
    height: entries.height + 5
    color: backgroundColor
    border.width: 2
    border.color: "#000000"

    Grid {
        id: entries
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 2
        anchors.leftMargin: 2
        spacing: 1
        rows: 9
        flow: Grid.TopToBottom

        ContextMenuEntry {
            id: moveFastButton
            buttonText: qsTr("Move fast")
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: moveFastButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: moveButton
            buttonText: qsTr("Move")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: moveButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: sneakButton
            buttonText: qsTr("Sneak")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: sneakButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: smokeButton
            buttonText: qsTr("Smoke")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: smokeButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: attackButton
            buttonText: qsTr("Attack")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: attackButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: ambushButton
            buttonText: qsTr("Ambush")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: ambushButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: defendButton
            buttonText: qsTr("Defend")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: defendButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: stopButton
            buttonText: qsTr("Stop")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: stopButton.entryClicked.connect(prepareSignal);
        }

        ContextMenuEntry {
            id: followButton
            buttonText: qsTr("Follow")
            width: buttonWidth
            height: buttonHeight
            backgroundColor: entryBackgroundColor

            Component.onCompleted: followButton.entryClicked.connect(prepareSignal);
        }
    }
}
