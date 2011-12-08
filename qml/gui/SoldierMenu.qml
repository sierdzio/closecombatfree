import QtQuick 1.1
import "menuEntries"

Rectangle {
    property int entryWidth: 175
    property int entryHeight: 54
    property int rows: 4
    property bool empty: true
    property color backgroundColor: "#7e8c24"
    property variant soldiersList

    id: root
    height: (((soldiers.cellHeight) * rows) + 3)
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
            width: entryWidth
            height: entryHeight
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

        anchors.top: parent.top
        anchors.left: parent.left
        height: parent.height
        width: {
            if (count > 8) {
                return (cellWidth * 3) + 3;
            } else if (count > 4) {
                return (cellWidth * 2) + 3;
            } else {
                return  cellWidth + 3;
            }
        }
        cellWidth: entryWidth + 2
        cellHeight: entryHeight + 2
        flow: GridView.TopToBottom

        model: soldierModel
        delegate: soldierDelegate
    }
}
