import QtQuick 1.1
import "../gui"
import "../engineLogicHelpers.js" as Logic
import "engineActionLogic.js" as ActionLogic

Item {
    property string unitType: "Generic unit"
    property string unitLogo: "../img/units/generic_unit_logo.png"
    property string unitStatus: "READY"
    property int unitIndex: -1

    property int rotationSpeed: 1 // seconds needed for half rotation (180 deg)
    property int turretRotationSpeed: 1 // seconds needed for half rotation (180 deg)
    property int maxSpeed: 1 // km/h - not really, this will be tweaked later
    property int acceleration: 1 // m/s^2 (maybe... could be arbitrary - to be decided later)
    property int unitWidth: 1 // mm - should be properly recalculated later on
    property int unitHeight: 1 // mm - should be properly recalculated later on

    property real moveFastFactor: 1.5 // Factor of movement speed (maxSpeed)
    property real sneakFactor: 0.4 // Factor of movement speed (maxSpeed)
    property int centerX: unitWidth/2
    property int centerY: unitHeight/2

    property int __tempX: x
    property int __tempY: y
    property string scheduledOperation;
    property bool selected: false

    signal unitStatusChanged(string newStatus)
    signal actionFinished(int index, real targetX, real targetY)

    signal moveTo (real newX, real newY)
    onMoveTo: ActionLogic.moveTo(newX, newY);

    signal moveFastTo (real newX, real newY)
    onMoveFastTo: ActionLogic.moveFastTo(newX, newY);

    signal sneakTo (real newX, real newY)
    onSneakTo: ActionLogic.sneakTo(newX, newY);

    width: unitWidth
    height: unitHeight

    function changeStatus(newStatusMessage) {
        unitStatus = newStatusMessage;
        unitStatusChanged(newStatusMessage);
    }

    UnitSelectionBox {
        id: selectionBox
        unitWidth: parent.unitWidth
        unitHeight: parent.unitHeight
        visible: selected
    }

    DefenceSphere {
        id: defenceSphere
        z: hull.z - 1

        visible: {
            if (defenceSphereColor == "") {
                return false;
            } else {
                return true;
            }
        }
        imageSource: {
            if (defenceSphereColor == "")
                return "";
            else
                return "../img/gui/defenceSphere_" + defenceSphereColor + ".png"

        }
        anchors {
            top: hull.top
            left: hull.left
            topMargin: 0
            leftMargin: centerX
        }

        transform: Rotation {
            origin.x: 0
            origin.y: defenceSphere.height/2
            angle: defenceSphereRotation
        }
    }

    Behavior on rotation {
        RotationAnimation {
            id: rotationAnimation
            property: "rotation"
            duration: 2000
            direction: RotationAnimation.Shortest
            easing.type: Easing.InOutQuad
            onRunningChanged: { // Not sure whether doing that will be good under all circumstances
                if (!rotationAnimation.running) {
                    x = __tempX;
                    y = __tempY;
                }
            }
        }
    }

    Behavior on x {
        NumberAnimation {
            id: xMoveAnimation
            duration: 2500
            easing.type: Easing.InOutQuad
            onRunningChanged: {
                if (!xMoveAnimation.running)
                    changeStatus("READY");
            }
        }
    }

    Behavior on y {
        NumberAnimation {
            id: yMoveAnimation
            duration: 2500
            easing.type: Easing.InOutQuad
        }
    }
}
