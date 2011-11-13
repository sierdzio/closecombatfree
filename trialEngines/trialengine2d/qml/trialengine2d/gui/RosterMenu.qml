import QtQuick 1.1

Rectangle {
    property int __entryWidth: 125
    property int __entryHeight: 50

    signal rosterEntryClicked(string rosterEntry)

    id: root
    height: 200
    width: 800
    color: "#7e8c24"
    border.color: "#1e1c00"
    border.width: 2
    // This is (temporarily) done in a very, very crude fashion.
    RosterMenuEntry {
        id: unit1
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 2
        anchors.topMargin: 2
        entryText: "Tank 1"

        width: __entryWidth
        height: __entryHeight
    }
    RosterMenuEntry {
        id: unit2
        anchors.left: parent.left
        anchors.top: unit1.bottom
        anchors.leftMargin: 2
        anchors.topMargin: 2

        width: __entryWidth
        height: __entryHeight
    }
    RosterMenuEntry {
        id: unit3
        anchors.left: parent.left
        anchors.top: unit2.bottom
        anchors.leftMargin: 2
        anchors.topMargin: 2

        width: __entryWidth
        height: __entryHeight
    }
    RosterMenuEntry {
        id: unit4
        anchors.left: parent.left
        anchors.top: unit3.bottom
        anchors.leftMargin: 2
        anchors.topMargin: 2

        width: __entryWidth
        height: __entryHeight
    }
    RosterMenuEntry {
        id: unit5
        anchors.left: unit1.right
        anchors.top: parent.top
        anchors.leftMargin: 2
        anchors.topMargin: 2

        width: __entryWidth
        height: __entryHeight
    }
    RosterMenuEntry {
        id: unit6
        anchors.left: unit2.right
        anchors.top: unit5.bottom
        anchors.leftMargin: 2
        anchors.topMargin: 2

        width: __entryWidth
        height: __entryHeight
    }

    function populateUnits(unitsList) {
        for (var i = 0; i < unitsList.length; i++) {
            var currentUnit = unitsList[i];
            root.children[i].entryText = currentUnit.unitType;
            root.children[i].entryLogo = currentUnit.unitLogo;
        }
    }
}
