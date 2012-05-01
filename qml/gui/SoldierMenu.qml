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

Rectangle {
    property int entryWidth: 175
    property int entryHeight: 54
    property int rows: 4
    property bool empty: true
    property color backgroundColor: "#7e8c24"
    property variant soldiersList

    id: root
    height: soldiers.height
    width: soldiers.width
    color: backgroundColor
    border.color: "#1e1c00"
    border.width: 2

    function populateSoldiers(tmpSoldiersList) {
        clear();
        soldiersList = tmpSoldiersList;
        for (var i = 0; i < soldiersList.length; i++) {
            var currentSoldier = soldiersList[i];
            soldierModel.append({"unitType": currentSoldier.name + "\n" + currentSoldier.role,
                                 "unitLogo": currentSoldier.soldierLogo,
                                 "unitStatus": currentSoldier.status});
            currentSoldier.unitStatusChanged.connect(changeStatus);
        }
        empty = false;

        // Get widest and highest dimension from cells:
        var widestCell = 0;
        var heighestCell = 0;
        soldierModel.get(0).unitStatus = "MOVING FAST";

        soldiers.currentIndex = 0;
        var current = soldiers.currentItem;
        widestCell = current.width;
        heighestCell = current.height;
        soldierModel.get(0).unitStatus = "READY";

        // Set all elements' dimentions
        for (var i = 0; i < soldiers.count; i++) {
            soldiers.currentIndex = i;
            var current = soldiers.currentItem;
            current.height = heighestCell;
            current.width = widestCell;
        }

        // Set GridView properties
        soldiers.currentIndex = -1;
        soldiers.cellWidth = widestCell + 2;
        soldiers.cellHeight = heighestCell + 2;
    }

    function clear() {
        soldierModel.clear();
        empty = true;
    }

    function changeStatus(newStatus, index) {
        unitModel.set(index, {"unitStatus": newStatus});
    }

    ListModel {
        id: soldierModel
    }

    Component {
        id: soldierDelegate

        RosterMenuEntry {
            backgroundColor: root.backgroundColor

            entryText: unitType
            entryLogo: unitLogo
            entryStatusText: unitStatus
            mouseAreaEnabled: false
        }
    }

    GridView {
        id: soldiers
        anchors.topMargin: 2
        anchors.leftMargin: 2
        interactive: false

        anchors.top: parent.top
        anchors.left: parent.left
        height: (cellHeight * 4) + 3
        width: {
            if (count > 8) {
                return (cellWidth * 3) + 3;
            } else if (count > 4) {
                return (cellWidth * 2) + 3;
            } else {
                return  cellWidth + 3;
            }
        }
        flow: GridView.TopToBottom

        model: soldierModel
        delegate: soldierDelegate
    }
}
