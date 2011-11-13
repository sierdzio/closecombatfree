import QtQuick 1.1

Item {
    property string scenarioFile: "Scenario_tst1.qml"

    Loader {
        id: map
        source: units.item.mapFile
        anchors.fill: parent
    }

    Loader {
        id: units
        source: scenarioFile
        anchors.fill: parent
    }

    function childAt(x, y) {
        return units.item.childAt(x, y);
    }
}
