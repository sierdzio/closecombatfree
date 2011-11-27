import QtQuick 1.1
import "../scenarios"

Rectangle {
    id: root
    width: windowWidth
    height: windowHeight
    color: "lightgray"

    Component.onCompleted: {
        console.log("Configuration from config file:\n" + configurationString());
    }

    Scenario {
        id: scenario
        anchors.fill: parent
        scenarioFile: "Scenario_tst1.qml"
    }
}
