import QtQuick 1.1
import "../gui"
import "../engineLogicHelpers.js" as LogicHelpers
import "engineScenarioLogic.js" as ScenarioLogic

Item {
    property string scenarioFile: "Scenario_tst1.qml"
    property string selectionMode: "DEFAULT" // Will be used for mobiles
    property int __aimLineRotation: 0
    property int __unitIndex: -1

    focus: true;

    Keys.onPressed: {
        if (event.modifiers == Qt.ControlModifier) {
            var digit = digitPressed(event);
            if (digit != -1)
                ScenarioLogic.groupUnits(digit);
//            event.accepted = true;
        } else {
            var digit = digitPressed(event);
            if (digit != -1)
                ScenarioLogic.selectGroup(digit);
        }
    }

    function digitPressed(event) {
        var result = -1;

        if (event.key == Qt.Key_1)
            result = 1;
        else if (event.key == Qt.Key_2)
            result = 2;
        else if (event.key == Qt.Key_3)
            result = 3;
        else if (event.key == Qt.Key_4)
            result = 4;
        else if (event.key == Qt.Key_5)
            result = 5;
        else if (event.key == Qt.Key_6)
            result = 6;
        else if (event.key == Qt.Key_7)
            result = 7;
        else if (event.key == Qt.Key_8)
            result = 8;
        else if (event.key == Qt.Key_9)
            result = 9;
        else if (event.key == Qt.Key_0)
            result = 10;

        return result;
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
            ScenarioLogic.rotateAimLine();
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
