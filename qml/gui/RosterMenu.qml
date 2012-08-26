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
import "../../qml/gui/menuEntries"
import "../../qml/units"

Rectangle {
    property int entryWidth: 175
    property int entryHeight: 54
    property int rows: 4
    property color backgroundColor: "#7e8c24"
    property variant unitsList

    id: root
    width: units.width
    height: (entryHeight * 4) + 3

    color: backgroundColor
    border.color: "#1e1c00"
    border.width: 2

    function populateUnits(tmpUnitsList) {
        unitsList = tmpUnitsList;

        for (var i = 0; i < unitsList.length; i++) {
            var currentUnit = unitsList[i];
            unitModel.append({"unitType": currentUnit.unitType,
                                 "unitLogo": currentUnit.unitLogo,
                                 "unitStatus": currentUnit.unitStatus,
                                 "unitSelected": currentUnit.selected});
//            currentUnit['unitStatusChanged(QString, int)'].connect(changeStatus); // TODO: Refactor for Qt5
            currentUnit.selectedChanged.connect(selectionChanged);
        }

        // Get widest and highest dimension from cells:
        var widestCell = 0;
        var heighestCell = 0;
        unitModel.get(0).unitStatus = "MOVING FAST";

        units.currentIndex = 0;
        var current = units.currentItem;
        widestCell = current.width;
        heighestCell = current.height;
        unitModel.get(0).unitStatus = "READY";

        // Set all elements' dimentions
        for (var i = 0; i < units.count; i++) {
            units.currentIndex = i;
            var current = units.currentItem;
            current.height = heighestCell;
            current.width = widestCell;
        }

        // Set GridView properties
        units.currentIndex = -1;
        units.cellWidth = widestCell + 2;
        units.cellHeight = heighestCell + 2;
        entryHeight = units.cellHeight;
        entryWidth = units.cellWidth
    }

    function getUnitAt(x, y) {
        var i = units.indexAt(x, y);

        if (i == -1)
            return i;

        if (unitsList.length <= i)
            return -1;

        return unitsList[i];
    }

    function childCenterCoords(x, y) {
        var index;
        index = units.indexAt(x, y);

        var row;
        row = index % rows;
        var column;
        column = index / rows;

        var result = {"x": ((column * units.cellWidth) + (units.cellWidth/2)),
            "y": ((row * units.cellHeight) + (units.cellHeight/2))};

        return result;
    }

    function changeStatus(newStatus, index) {
        unitModel.set(findListIndex(index), {"unitStatus": newStatus});
    }

    function selectionChanged(state, index) {
        unitModel.set(findListIndex(index), {"unitSelected": state});
    }

    function findListIndex(unitIndex) {
        for (var i = 0; i < units.count; ++i) {
            if (unitsList[i].unitIndex == unitIndex) {
                return i;
            }
        }
    }

    ListModel {
        id: unitModel
    }

    Component {
        id: unitDelegate

        RosterMenuEntry {
            backgroundColor: root.backgroundColor

            entryText: unitType
            entryLogo: unitLogo
            entryStatusText: unitStatus
            selected: unitSelected
        }
    }

    GridView {
        id: units
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
