import QtQuick 1.1
import "engineRosterLogic.js" as RosterLogic

Rectangle {
    property int entryWidth: 175
    property int entryHeight: 54

    id: root
    height: (unit0.height * 4) + (units.spacing * 6)
    width: 800
    color: "#7e8c24"
    border.color: "#1e1c00"
    border.width: 2

    function populateUnits(tmpUnitsList) {
        RosterLogic.unitsList = tmpUnitsList;
        for (var i = 0; i < RosterLogic.unitsList.length; i++) {
            var currentUnit = RosterLogic.unitsList[i];
            units.children[i].entryText = currentUnit.unitType;
            units.children[i].entryLogo = currentUnit.unitLogo;
            units.children[i].changeStatus(currentUnit.unitStatus);
            currentUnit.unitStatusChanged.connect(units.children[i].changeStatus);
            currentUnit.selectionChanged.connect(units.children[i].selectionChanged);
        }
    }

    function getUnitAt(x, y) {
        var i = -1;
        var child = units.childAt(x, y);

        if ((child != null)) {
            i = child.index;
        } else {
            return -1;
        }

        if (RosterLogic.unitsList.length <= i)
            return -1;

        return RosterLogic.unitsList[i];
    }

    function childAt(x, y) {
        return units.childAt(x, y);
    }

    Grid {
        id: units
        anchors.fill: parent
        anchors.topMargin: 2
        anchors.leftMargin: 2
        spacing: 2
        rows: 4
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
