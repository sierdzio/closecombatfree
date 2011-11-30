import QtQuick 1.1
import "engineRosterHelper.js" as SoldierHelper

Rectangle {
    property int entryWidth: 175
    property int entryHeight: 54

    id: root
    height: (unit0.height * 4) + (units.spacing * 6)
    width: 600
    color: "#7e8c24"
    border.color: "#1e1c00"
    border.width: 2

    function populateSoldiers(tmpSoldiersList) {
        clear();
        SoldierHelper.unitsList = tmpSoldiersList;
        for (var i = 0; i < SoldierHelper.unitsList.length; i++) {
            var currentUnit = SoldierHelper.unitsList[i];
            units.children[i].entryText = currentUnit.name + "\n" + currentUnit.role;
            units.children[i].entryLogo = currentUnit.soldierLogo;
            units.children[i].changeStatus(currentUnit.status);
            currentUnit.unitStatusChanged.connect(units.children[i].changeStatus);
        }
    }

    function clear() {
        SoldierHelper.unitsList = new Array();
        for (var i = 0; i < 8; i++) {
            if (units.children[i].entryText != "") {
                units.children[i].entryText = "";
                units.children[i].entryLogo = "";
                units.children[i].entryStatusText = "";
            }
        }
    }

    Grid {
        id: units
        anchors.fill: parent
        anchors.topMargin: 2
        anchors.leftMargin: 2
        spacing: 2
        rows: 4
        columns: 4
        flow: Grid.TopToBottom

        RosterMenuEntry {
            id: unit0
            index: 0

            width: entryWidth
            height: entryHeight
        }
        RosterMenuEntry {
            id: unit1
            index: 1

            width: entryWidth
            height: entryHeight
        }
        RosterMenuEntry {
            id: unit2
            index: 2

            width: entryWidth
            height: entryHeight
        }
        RosterMenuEntry {
            id: unit3
            index: 3

            width: entryWidth
            height: entryHeight
        }
        RosterMenuEntry {
            id: unit4
            index: 4

            width: entryWidth
            height: entryHeight
        }
        RosterMenuEntry {
            id: unit5
            index: 5

            width: entryWidth
            height: entryHeight
        }
        RosterMenuEntry {
            id: unit6
            index: 6

            width: entryWidth
            height: entryHeight
        }
        RosterMenuEntry {
            id: unit7
            index: 7

            width: entryWidth
            height: entryHeight
        }
        RosterMenuEntry {
            id: unit8
            index: 8

            width: entryWidth
            height: entryHeight
        }
    }
}
