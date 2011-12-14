import QtQuick 1.1
import "qrc:/skin"
import "qrc:/core/engineLogicHelpers.js" as Logic
import "qrc:/core/engineActionLogic.js" as ActionLogic

Item {
    property string unitType: "Generic unit"
    property string unitLogo: "qrc:/img/units/generic_unit_logo.png"
    property string unitStatus: "READY"
    property int groupNumber: 0 // For now, a unit can be only in one group
    property int unitIndex: -1

    property int rotationSpeed: 1 // seconds needed for half rotation (180 deg)
    property int turretRotationSpeed: 1 // seconds needed for half rotation (180 deg)
    property int maxSpeed: 1 // km/h - not really, this will be tweaked later
    property int acceleration: 1 // m/s^2 (maybe... could be arbitrary - to be decided later)
    property int unitWidth: 1 // mm - should be properly recalculated later on
    property int unitHeight: 1 // mm - should be properly recalculated later on
    property list<Soldier> soldiers //Soldiers.soldiers

    property real moveFastFactor: 1.5 // Factor of movement speed (maxSpeed)
    property real sneakFactor: 0.4 // Factor of movement speed (maxSpeed)
    property int centerX: unitWidth/2
    property int centerY: unitHeight/2

    property int __tempX: x
    property int __tempY: y
    property string scheduledOperation;
    property bool selected: false
    property bool firing: false
    property bool smoking: false
    property int defenceSphereRotation: 0
    property string defenceSphereColor: ""
    property bool paused: false

    signal togglePause ()
    onTogglePause: {
        if (paused == true) {
            paused = false;
        } else {
            paused = true;
        }
    }

    signal unitStatusChanged(string newStatus, int index)
    signal actionFinished(int index, real targetX, real targetY)

    signal moveTo (real newX, real newY)
    onMoveTo: ActionLogic.moveTo(newX, newY);

    signal moveFastTo (real newX, real newY)
    onMoveFastTo: ActionLogic.moveFastTo(newX, newY);

    signal sneakTo (real newX, real newY)
    onSneakTo: ActionLogic.sneakTo(newX, newY);

    signal selectionChanged(bool state, int index)
    onSelectedChanged: selectionChanged(selected, unitIndex);

    signal cancelOrder ()
    onCancelOrder: ActionLogic.cancelOrder();

    id: root
    width: unitWidth
    height: unitHeight

    signal positionChanged(real x, real y, int index)

    onXChanged: {
        if (Math.round(x % 10) === 0) {
            positionChanged(x, y, unitIndex);
        }
    }

    onYChanged: {
        if (Math.round(y % 10) === 0) {
            positionChanged(x, y, unitIndex);
        }
    }

    function changeStatus(newStatusMessage) {
        unitStatus = newStatusMessage;
        unitStatusChanged(newStatusMessage, unitIndex);
    }

    Text {
        id: groupMembership
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        z: 50

        height: 10
        width: 10
        color: "#ffffff"
        text: (groupNumber == 0) ? "" : groupNumber
        font.pointSize: 10
        style: Text.Raised
        font.bold: true
        font.family: "Ubuntu Condensed"
        verticalAlignment: Text.Bottom
        horizontalAlignment: Text.AlignRight
        rotation: -root.rotation
    }

    UnitSelectionBox {
        id: selectionBox
        unitWidth: parent.unitWidth
        unitHeight: parent.unitHeight
        visible: selected
        z: 3
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
                return "qrc:/img/gui/defenceSphere_" + defenceSphereColor + ".png"

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

    RotationAnimation on rotation {
        id: rotationAnimation
        duration: 2000
        direction: RotationAnimation.Shortest
        easing.type: Easing.InOutQuad
        paused: running? root.paused : false

        onRunningChanged: { // Not sure whether doing that will be good under all circumstances
            if ((!rotationAnimation.running) && (unitStatus != "STOPPED")) {
                xMoveAnimation.to = __tempX;
                xMoveAnimation.running = true;
                yMoveAnimation.to = __tempY;
                yMoveAnimation.running = true;
            }
        }
    }

    NumberAnimation on x {
        id: xMoveAnimation
        duration: 2500
        easing.type: Easing.InOutQuad
        paused: running? root.paused : false

        onRunningChanged: {
            if ((!xMoveAnimation.running) && (unitStatus != "STOPPED")) {
                // Warning! This order is important for order markers!
                changeStatus("READY");
                actionFinished(unitIndex, __tempX, __tempY);
            } else if (unitStatus == "STOPPED") {
                changeStatus("READY");
            }
        }
    }

    NumberAnimation on y {
        id: yMoveAnimation
        duration: 2500
        easing.type: Easing.InOutQuad
        paused: running? root.paused : false
    }

}
