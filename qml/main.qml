import QtQuick 1.1
import "../qml/menus"

Rectangle {
    id: root
    width: Config.windowWidth
    height: Config.windowHeight
    color: "#7e8c24"

//    Component.onCompleted: {
////        console.log("Configuration from config file:\n" + configurationString());
//        console.log(Config.windowWidth + "x" + Config.windowHeight);
//    }

    ScenarioMenu {
        id: scenarioMenu
        anchors.fill: parent
    }
}
