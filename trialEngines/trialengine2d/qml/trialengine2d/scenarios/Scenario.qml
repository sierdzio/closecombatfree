import QtQuick 1.1
import "../gui"
import "../engineLogicHelpers.js" as LogicHelpers
import "engineScenarioLogic.js" as ScenarioLogic

Item {
    property string scenarioFile: "Scenario_tst1.qml"
    property int __aimLineRotation: 0
    property int __unitIndex: -1

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

            Component.onCompleted: {
                for (var i = 0; i < units.item.children.length; i++) {
                    units.item.children[i].unitIndex = i;
                }
            }
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
        id: rotationTimer // this should be changed
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
        property string __fireAnimationString: "gun_fire"

        id: fireTimer
        interval: 80
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            ScenarioLogic.switchFireFrame(__fireAnimationString);
        }
    }

    function childAt(x, y) {
        return units.item.childAt(x, y);
    }

    function childIndexAt(x, y) {
        var result = 0;
        var child = units.item.childAt(x, y);

        for (var i = 0; i < units.item.children.length; i++) {
            if (child == units.item.children[i]) {
                result = i;
                break;
            }
        }

        return result;
    }

    function childIndex(child) {
        var result = 0;

        for (var i = 0; i < units.item.children.length; i++) {
            if (child == units.item.children[i]) {
                result = i;
                break;
            }
        }

        return result;
    }
}
