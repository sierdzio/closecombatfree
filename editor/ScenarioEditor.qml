import QtQuick 1.1
import "../qml/gui"
import "../qml/units/engineActionLogic.js" as LogicHelpers
import "../qml/scenarios/engineScenarioLogic.js" as ScenarioLogic

// Elements available for editors.
import "../maps"
import "../maps/props"
import "../scenarios"
import "../saves"
import "../units"
import "../units/tanks"
import "../units/tanks/tests"
import "../qml"
import "../qml/maps"
import "../qml/maps/props"
import "../qml/scenarios"
import "../qml/units"
import "../qml/units/tanks"

Rectangle {
    property string playerSide: "neutral" // Experimental!
    property color menuBackgroundColor: "#7e8c24"

    id: root
    color: menuBackgroundColor

    width: map.width
    height: map.height

    Map {
        id: map
        hipsometricImage: "../../img/maps/hipsometric_default.png"
        backgroundImage: "../../img/maps/test1.jpg"
        anchors.fill: parent
    }
}

