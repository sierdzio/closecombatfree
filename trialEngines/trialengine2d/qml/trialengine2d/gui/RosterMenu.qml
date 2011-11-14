import QtQuick 1.1
import "engineRosterLogic.js" as RosterLogic

Rectangle {
    property int entryWidth: 175
    property int entryHeight: 50

    id: root
    height: 200
    width: 800
    color: "#7e8c24"
    border.color: "#1e1c00"
    border.width: 2

    function populateUnits(tmpUnitsList) {
        RosterLogic.unitsList = tmpUnitsList;
        for (var i = 0; i < RosterLogic.unitsList.length; i++) {
            var currentUnit = RosterLogic.unitsList[i];
            root.children[i].entryText = currentUnit.unitType;
            root.children[i].entryLogo = currentUnit.unitLogo;
            root.children[i].changeStatus(currentUnit.unitStatus);
            currentUnit.unitStatusChanged.connect(root.children[i].changeStatus);
        }
    }

    function getUnitAt(x, y) {
        var i = 0;
        if (childAt(x, y) != null) {
            i = childAt(x, y).index;
        } else {
            return 0;
        }

        return RosterLogic.unitsList[i];
    }

    // This is (temporarily) done in a very, very crude fashion.
    RosterMenuEntry {
        id: unit0
        index: 0
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 2
        anchors.topMargin: 2

        width: entryWidth
        height: entryHeight
    }
    RosterMenuEntry {
        id: unit1
        index: 1
        anchors.left: parent.left
        anchors.top: unit0.bottom
        anchors.leftMargin: 2
//        anchors.topMargin: 2

        width: entryWidth
        height: entryHeight
    }
    RosterMenuEntry {
        id: unit2
        index: 2
        anchors.left: parent.left
        anchors.top: unit1.bottom
        anchors.leftMargin: 2
//        anchors.topMargin: 2

        width: entryWidth
        height: entryHeight
    }
    RosterMenuEntry {
        id: unit3
        index: 3
        anchors.left: parent.left
        anchors.top: unit2.bottom
        anchors.leftMargin: 2
//        anchors.topMargin: 2

        width: entryWidth
        height: entryHeight
    }
    RosterMenuEntry {
        id: unit4
        index: 4
        anchors.left: unit0.right
        anchors.top: parent.top
        anchors.leftMargin: 2
//        anchors.topMargin: 2

        width: entryWidth
        height: entryHeight
    }
    RosterMenuEntry {
        id: unit5
        index: 5
        anchors.left: unit1.right
        anchors.top: unit4.bottom
        anchors.leftMargin: 2
//        anchors.topMargin: 2

        width: entryWidth
        height: entryHeight
    }
    RosterMenuEntry {
        id: unit6
        index: 6
        anchors.left: unit2.right
        anchors.top: unit5.bottom
        anchors.leftMargin: 2
//        anchors.topMargin: 2

        width: entryWidth
        height: entryHeight
    }
    RosterMenuEntry {
        id: unit7
        index: 7
        anchors.left: unit3.right
        anchors.top: unit6.bottom
        anchors.leftMargin: 2
//        anchors.topMargin: 2

        width: entryWidth
        height: entryHeight
    }
    RosterMenuEntry {
        id: unit8
        index: 8
        anchors.left: unit4.right
        anchors.top: parent.top
        anchors.leftMargin: 2
//        anchors.topMargin: 2

        width: entryWidth
        height: entryHeight
    }
}
