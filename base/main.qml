import QtQuick 1.1
import "../scenarios"

Rectangle {
    id: root
    width: configWindowWidth
    height: configWindowHeight
    color: "lightgray"

//    Component.onCompleted: {
//        console.log("Configuration from config file:\n" + configurationString());
//    }

    Scenario {
        id: scenario
        anchors.fill: parent
        scenarioFile: "Scenario_tst1.qml"
    }
}
