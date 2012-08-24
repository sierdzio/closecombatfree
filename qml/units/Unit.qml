/****************************************************************************
** Close Combat Free is a free, easily moddable CC-like game.
** Copyright (C) 2011 Tomasz Siekierda
** Email: sierdzio@gmail.com, website: http://www.sierdzio.com
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program (in doc/ directory).
** If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
****************************************************************************/

import QtQuick 2.0
import "../../qml/gui"
import "../../qml/units"
import "../../qml/units/engineActionLogic.js" as ActionLogic

Item {
    property string objectType: "unit"
    // Sadly, this is needed for file saving:
    property string unitFileName: "Unit"
    property string unitType: "Generic unit"
    property string unitLogo: "../../img/units/generic_unit_logo.png"
    property string unitStatus: "READY"
    property string unitSide: "neutral"
    property int groupNumber: 0 // For now, a unit can be only in one group
    property int unitIndex: -1
    property bool sideMarkVisible: true
    property string sideMarkSource: "../../img/units/sideMarks/sideMark_side_1.png"
    // Defines a set of side marks to be used to decorate this unit.
    // Side mark filenames should be made according to this template:
    // <root>/img/units/sideMarks/sideMark_<sideMarkSet>_<sideNumber>.png
    property string sideMarkSet: "side"

    property int rotationSpeed: 1 // seconds needed for half rotation (180 deg)
    property int turretRotationSpeed: 1 // seconds needed for half rotation (180 deg)
    property int maxSpeed: 1 // km/h - not really, this will be tweaked later
    property int acceleration: 1 // m/s^2 (maybe... could be arbitrary - to be decided later)
    property int unitWidth: 1 // mm - should be properly recalculated later on
    property int unitHeight: 1 // mm - should be properly recalculated later on
    property list<Soldier> soldiers //Soldiers.soldiers
//    property var orderMarkers: new Array
    property var orders: new Array;

//    function setOrderMarker(index, orderNumber, orderName) {
//        // This component renders an order marker.
//        var component = Qt.createComponent("../../qml/gui/OrderMarker.qml");

//        if (component.status === Component.Ready) {
//            var marker = component.createObject(root);
//            marker.visible = true;
//            marker.index = index;
//            marker.number = orderNumber;
////            marker.dragComplete.connect(modifyTargetFromMarker);
//            marker.orderColor = EngineHelpers.colorForOrder(orderName);

//            orders.push(marker);
//            return marker;
//        } else {
//            return 0;
//        }

////        marker.x = (targetX - marker.centerX);
////        marker.y = (targetY - marker.centerY);
////        marker.visible = true;
//    }

    property real moveFastFactor: 1.5 // Factor of movement speed (maxSpeed)
    property real sneakFactor: 0.4 // Factor of movement speed (maxSpeed)
    property int centerX: unitWidth/2
    property int centerY: unitHeight/2

    property int __tempX: x
    property int __tempY: y
    property string scheduledOperation;
    property int currentOrder: -1
    property bool selected: false
    property bool firing: false
    property bool smoking: false
    property int defenceSphereRotation: 0
    property string defenceSphereColor: ""
    property bool paused: false
    property bool moving: false
//    property var orderQueue: new Array;

    signal togglePause ()
    onTogglePause: {
        if (paused == true) {
            paused = false;
        } else {
            paused = true;
        }
    }

    signal movementStateChange(bool movingState, int unitIndex)
    onMovingChanged: {
        movementStateChange(moving, unitIndex);
    }

    signal unitStatusChanged (string newStatus, int index)
    signal actionFinished (int index, real targetX, real targetY)
    signal queueOrderFinished ()

    signal moveTo (real newX, real newY, var parent)
    onMoveTo: ActionLogic.moveTo(newX, newY, parent);

    signal moveFastTo (real newX, real newY, var parent)
    onMoveFastTo: ActionLogic.moveFastTo(newX, newY, parent);

    signal sneakToSignal (real newX, real newY, var parent)
//    onSneakTo: ActionLogic.sneakTo(newX, newY);
    function sneakTo(newX, newY, parent) {
        sneakToSignal(newX, newY, parent);
        ActionLogic.sneakTo(newX, newY, parent);
    }

    signal selectionChanged (bool state, int index)
    onSelectedChanged: selectionChanged(selected, unitIndex);

    signal cancelOrder ()
    onCancelOrder: ActionLogic.cancelOrder();

    // TODO: investigate why signals don't work properly
//    signal queueOrder (string orderName, real newX, real newY)
//    onQueueOrder: ActionLogic.queueOrder(orderName, newX, newY);
    function queueOrder(orderName, newX, newY, parent) { ActionLogic.queueOrder(orderName, newX, newY, parent); }

    signal processQueue ()
    onProcessQueue: ActionLogic.processQueue();

    signal continueQueue ()
    onContinueQueue: ActionLogic.continueQueue();

    // TODO: investigate why signals don't work properly
//    signal hit(string byWhat, real xWhere, real yWhere) // should be variant.
//    onHit: ActionLogic.hit(byWhat, xWhere, yWhere);
    function hit(byWhat, xWhere, yWhere) { ActionLogic.hit(byWhat, xWhere, yWhere); }

    id: root
    width: unitWidth
    height: unitHeight
    state: "healthy"

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

    Component.onCompleted: {
        queueOrderFinished.connect(continueQueue);

        // Code that displays all info on unit's states:
//        var result = "Unit: " + unitType + "\n";
//        for (var i = 0; i < states.length; i++) {
//            state = states[i].name;
//            result += "State: " + states[i].name + ", temp text: " + temp + "\n";
//        }
//        result += "\n";
//        console.log(result);
    }

    function changeStatus(newStatusMessage) {
        unitStatus = newStatusMessage;
        unitStatusChanged(newStatusMessage, unitIndex);
    }

    function clearOrderQueue() {
        for (var i = 0; i < orders.length; ++i) {
            orders[i].destroy();
        }
        currentOrder = -1;

        return orders = new Array;
    }

    Text {
        id: groupMembership
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        z: 5

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
                return "../../img/gui/defenceSphere_" + defenceSphereColor + ".png"

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

    Image {
        id: sideMarkImage
        x: 0
        y: 0
        z: 5
        width: 10
        height: 10
        rotation: -root.rotation
        source: sideMarkSource
        visible: sideMarkVisible
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
            if (!xMoveAnimation.running) {
                moving = false;
            }

            if ((!xMoveAnimation.running) && (unitStatus != "STOPPED")) {
                // Warning! This order is important for order markers!
                changeStatus("READY");
                actionFinished(unitIndex, __tempX, __tempY);
                queueOrderFinished();
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

    //// Temp debug
    Rectangle {
        id: destroyedState
        visible: (parent.state === "healthy")? false : true;
        width: 10
        height: width
        radius: 10
        color: "#ff3333"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        z: 10
    }
}
