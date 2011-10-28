import QtQuick 1.1
import "../tanks"
import "../gui"
import "../maps"
import "../engineLogicHelpers.js" as LogicHelpers
import "../engineLogicCore.js" as LogicCore

Item {
    property string scenarioFile: "Scenario_tst1.qml"

    id: root
    height: map.height
    width: map.width

    Map {
        id: map
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
