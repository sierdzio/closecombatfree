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
import QmlBase 0.1

import "../../../qml/gui/menuEntries"
import "../../../qml/units"

BaseRosterMenu {
    property int entryWidth: 175
    property int entryHeight: 54
    property int rows: 4
    property color backgroundColor: "#7e8c24"
    property variant unitModel

    id: root
    width: unitsView.width
    height: (entryHeight * 4) + 3

    Component {
        id: unitDelegate

        RosterMenuEntry {
            backgroundColor: root.backgroundColor
            entryText: model.modelData.unitType
            entryLogo: model.modelData.unitLogo
            entryStatusText: model.modelData.unitStatus
            selected: model.modelData.selected
        }
    }

    Rectangle {
        id: background
        color: backgroundColor
        border.color: "#1e1c00"
        border.width: 2

        anchors.top: parent.top
        anchors.left: parent.left
        width: unitsView.width
        height: unitsView.height

        GridView {
            objectName: "unitsView"
            id: unitsView
            anchors.topMargin: 2
            anchors.leftMargin: 2
            interactive: false

            anchors.top: parent.top
            anchors.left: parent.left

            height: (cellHeight * 4) + 3
            width: (count > 8)? (cellWidth + 3) : (cellWidth * 2) + 3
            flow: GridView.TopToBottom

            model: unitModel
            delegate: unitDelegate
        }
    }
}
