import QtQuick 1.1
import "qrc:/skin/menuEntries"

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
        for (var i = 0; i < soldiers.count; i++) {
            soldiers.currentIndex = i;
            var current = soldiers.currentItem;
            if (current.height > heighestCell) {
                heighestCell = current.height;
            }

            if (current.width > widestCell) {
                widestCell = current.width;
            }
        }

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
