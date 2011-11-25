import QtQuick 1.1
import "../gui"
import "../base/engineLogicHelpers.js" as LogicHelpers
import "engineScenarioLogic.js" as ScenarioLogic

Item {
    property string scenarioFile: ""
    property int __aimLineRotation: 0
    property int __rubberBandRotation: 0
    property int __unitIndex: -1

    id: root
    focus: true;

    Component.onCompleted: ScenarioLogic.createOrderMarkers();

    Keys.onPressed: {
        if (event.modifiers == Qt.ControlModifier) {
            var digit = ScenarioLogic.digitPressed(event);
            if (digit != -1)
                ScenarioLogic.groupUnits(digit);
        } else {
            var digit = ScenarioLogic.digitPressed(event);
            if (digit != -1)
                ScenarioLogic.selectGroup(digit);
        }
    }

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
            onPressAndHold: {
                ScenarioLogic.handlePressAndHold(mouse);
            }
            onReleased: {
                ScenarioLogic.handleMouseReleased();
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

    RubberBand {
        id: rubberBand
        visible: false

        transform: Rotation {
            origin.x: 0
            origin.y: 0
            angle: __rubberBandRotation
        }
    }

//    Rectangle {
//        id: test1
//        color: "#ffff22"
//        width: 15
//        height: 15
//    }

//    Rectangle {
//        id: test2
//        color: "#ff22ff"
//        width: 15
//        height: 15
//    }

    Loader {
        id: contextLoader
        rotation: 0
        z: root.z + 1
    }

    // Timer for aimline rotation updates.
    Timer {
        id: rotationTimer
        interval: 120
        running: false
        repeat: true
        onTriggered: {
            ScenarioLogic.updateAimLine();
        }
    }

    // Timer for rubber band
    Timer {
//        property int __modifiers: Qt.NoModifier

        id: rubberBandTimer
        interval: 120
        running: false
        repeat: true
        onTriggered: {
            ScenarioLogic.updateRubberBand(mouseAreaMain.mouseX, mouseAreaMain.mouseY);
        }
    }

    // Needed for effectsContainer in JS file.
    // Would be neat to rethink and optimise that.
    Item {
        id: itemContainer
        visible: true
        anchors.fill: parent
    }

    // Timer for on-screen animations..
    Timer {
        id: effectsTimer
        interval: 80
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            ScenarioLogic.updateEffects();
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
