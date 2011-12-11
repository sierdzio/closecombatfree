import QtQuick 1.1
import "qrc:/skin/menuEntries"
import "qrc:/core/units"

Rectangle {
    property int entryWidth: 175
    property int entryHeight: 54
    property int rows: 4
    property color backgroundColor: "#7e8c24"
    property variant unitsList

    id: root
    width: units.width
//    height: units.height
//    width: entryWidth
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
            currentUnit.unitStatusChanged.connect(changeStatus);
            currentUnit.selectionChanged.connect(selectionChanged);
        }

        // Get widest and highest dimension from cells:
        var widestCell = 0;
        var heighestCell = 0;
        for (var i = 0; i < units.count; i++) {
            units.currentIndex = i;
            var current = units.currentItem;
            if (current.height > heighestCell) {
                heighestCell = current.height;
            }

            if (current.width > widestCell) {
                widestCell = current.width;
            }
        }

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
