import QtQuick 1.1
import "../scenarios"

Rectangle {
    id: root
    width: windowWidth
    height: windowHeight
    color: "lightgray"

    Scenario {
        id: scenario
        anchors.fill: parent
        scenarioFile: "Scenario_tst1.qml"
    }
}
