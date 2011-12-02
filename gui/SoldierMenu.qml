import QtQuick 1.1
import "engineRosterHelper.js" as SoldierHelper

Rectangle {
    property int entryWidth: 175
    property int entryHeight: 54
    property int rows: 4

    id: root
    height: (((soldiers.cellHeight) * rows) + 3)
    width: soldiers.width
    color: "#7e8c24"
    border.color: "#1e1c00"
    border.width: 2

    function populateSoldiers(tmpSoldiersList) {
        clear();
        SoldierHelper.unitsList = tmpSoldiersList;
        for (var i = 0; i < SoldierHelper.unitsList.length; i++) {
            var currentSoldier = SoldierHelper.unitsList[i];
            soldierModel.append({"unitType": currentSoldier.name + "\n" + currentSoldier.role,
                                 "unitLogo": currentSoldier.soldierLogo,
                                 "unitStatus": currentSoldier.status});
            currentSoldier.unitStatusChanged.connect(changeStatus);
        }
    }

    function clear() {
        SoldierHelper.unitsList = new Array();
        soldierModel.clear();
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
        width: (count > 8)? ((cellWidth * 3) + 3) : ((cellWidth * 2) + 3)
        cellWidth: entryWidth + 2
        cellHeight: entryHeight + 2
        flow: GridView.TopToBottom

        model: soldierModel
        delegate: soldierDelegate
    }
}
