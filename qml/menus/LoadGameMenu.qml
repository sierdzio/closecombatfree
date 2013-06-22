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
import "../../qml/gui/menuEntries"
import "../../qml/gui"

Rectangle {
    property color backgroundColor: "#bb333232"

    id: root
    color: backgroundColor
    border.color: Qt.tint(backgroundColor, "#88aaaaaa")
    border.width: 1

    width: Config.windowWidth
    height: Config.windowHeight
    radius: 20

    Component.onCompleted: {
        cancelButton.entryClicked.connect(toggleVisibility);
        entries.entryClicked.connect(gameEntryClicked);
    }

    function toggleVisibility() {
        root.visible = !root.visible;
    }

    signal gameEntryClicked (string gamePath)
    onGameEntryClicked: {
        //toggleVisibility();
        gameLoader.load(gamePath, true);
    }

    MouseArea {
        id: blockingArea
        anchors.fill: parent
    }

    LoadGameEntries {
        id: entries
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: buttons.top
        anchors.margins: parent.radius
    }

    // Bottom buttons
    Row {
        id: buttons

        spacing: 5
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: parent.radius
        anchors.rightMargin: parent.radius

        MenuEntry {
            id: cancelButton
            text: "Cancel"
        }
    }
}
