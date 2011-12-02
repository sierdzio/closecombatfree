import QtQuick 1.1
import "engineRosterHelper.js" as RosterHelper

Rectangle {
    property int entryWidth: 175
    property int entryHeight: 54

    id: root
    width: units.width
    height: (entryHeight + 2) * 4 + 3
    color: "#7e8c24"
    border.color: "#1e1c00"
    border.width: 2

    function populateUnits(tmpUnitsList) {
        RosterHelper.unitsList = tmpUnitsList;
        for (var i = 0; i < RosterHelper.unitsList.length; i++) {
            var currentUnit = RosterHelper.unitsList[i];
            unitModel.append({"unitType": currentUnit.unitType,
                                 "unitLogo": currentUnit.unitLogo,
                                 "unitStatus": currentUnit.unitStatus,
                                 "unitSelected": currentUnit.selected});
            currentUnit.unitStatusChanged.connect(changeStatus);
            currentUnit.selectionChanged.connect(selectionChanged);
        }
    }

    function getUnitAt(x, y) {
        var i = units.indexAt(x, y);

        if (i == -1)
            return i;

        if (RosterHelper.unitsList.length <= i)
            return -1;

        return RosterHelper.unitsList[i];
    }

    function childAt(x, y) {
        return units.childAt(x, y);
    }

    function changeStatus(newStatus, index) {
        unitModel.set(index, {"unitStatus": newStatus});
    }

    function selectionChanged(state, index) {
        unitModel.set(index, {"unitSelected": state});
    }

    ListModel {
        id: unitModel
    }

    Component {
        id: unitDelegate

        RosterMenuEntry {
            width: entryWidth
            height: entryHeight

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

        anchors.top: parent.top
        anchors.left: parent.left
        height: parent.height
        width: (count > 8)? (cellWidth + 3) : (cellWidth * 2) + 3
        cellWidth: entryWidth + 2
        cellHeight: entryHeight + 2
        flow: Grid.TopToBottom

        model: unitModel

        delegate: unitDelegate
    }
}
