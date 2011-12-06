import QtQuick 1.1
import "menuEntries"
import "../units/units.js" as Units

Rectangle {
    property int entryWidth: 175
    property int entryHeight: 54
    property int rows: 4
    property color backgroundColor: "#7e8c24"

    id: root
    width: units.width
    height: (entryHeight + 2) * rows + 3
    color: backgroundColor
    border.color: "#1e1c00"
    border.width: 2

    function populateUnits(tmpUnitsList) {
        Units.list = tmpUnitsList;
        for (var i = 0; i < Units.list.length; i++) {
            var currentUnit = Units.list[i];
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

        if (Units.list.length <= i)
            return -1;

        return Units.list[i];
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

        console.log("x: " + result.x + ", y " + result.y);
        return result;
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

        anchors.top: parent.top
        anchors.left: parent.left
        height: parent.height
        width: (count > 8)? (cellWidth + 3) : (cellWidth * 2) + 3
        cellWidth: entryWidth + 2
        cellHeight: entryHeight + 2
        flow: GridView.TopToBottom

        model: unitModel
        delegate: unitDelegate
    }
}
