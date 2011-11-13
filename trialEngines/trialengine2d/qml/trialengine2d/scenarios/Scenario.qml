import QtQuick 1.1
import "../gui"
import "../engineLogicHelpers.js" as LogicHelpers
import "engineScenarioLogic.js" as ScenarioLogic

Item {
    property string scenarioFile: "Scenario_tst1.qml"
    property int __aimLineRotation: 0

    Item {
        id: gameArea
        height: parent.height - roster.height
        width: parent.width

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

        MouseArea {
            id: mouseAreaMain
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            hoverEnabled: true
            z: -1

            onClicked: {
                ScenarioLogic.handleMouseClick(mouse);
            }
        }
    }

    RosterMenu {
        id: roster
        width: parent.width
        anchors.top: gameArea.bottom

        Component.onCompleted: {
            populateUnits(units.item.children);
        }

        MouseArea {
            id: mouseAreaRoster
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            hoverEnabled: true
            z: -1

            onClicked: {
                ScenarioLogic.handleMouseClickRoster(mouse);
            }
        }
    }

    Rectangle {
        id: aimLine
        z: root.z + 1
        visible: false
        width: 3
        height: 150

        transform: Rotation {
            origin.x: 1
            origin.y: 0
            angle: __aimLineRotation
        }
    }

    Loader {
        id: contextLoader
        rotation: 0
        z: root.z + 1
    }
    Timer {
        id: aimLineRotationTimer // this should be changed
        interval: 120
        running: false
        repeat: true
        onTriggered: {
            ScenarioLogic.rotateAimLine();
        }
    }

    Image {
        property int imageNumber: 0
        id: fireImage
        visible: true
        source: ""
        scale: 3
        z: 5
    }
    Timer {
        id: fireTimer
        interval: 80
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            ScenarioLogic.switchFireFrame("gun_fire");
        }
    }

    function childAt(x, y) {
        return units.item.childAt(x, y);
    }
}
