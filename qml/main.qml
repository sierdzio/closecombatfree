import QtQuick 1.1
import "qrc:/core/menus"

Rectangle {
    id: root
    width: configWindowWidth
    height: configWindowHeight
    color: "#7e8c24"

//    Component.onCompleted: {
//        console.log("Configuration from config file:\n" + configurationString());
//    }

    ScenarioMenu {
        id: scenarioMenu
        anchors.fill: parent
    }
}
