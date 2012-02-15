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

/*!
  \ingroup CloseCombatFree

  \class engineActionLogic

  JavaScript file containing logic responsible for performing Unit actions (moving,
  shooting, etc.).
  */

/*!
  \memberof engineActionLogic

  Queues the order to move at casual speed; starts processing of the queue.
  */
function moveTo (newX, newY) {
    queueOrder("Move", newX, newY);
    processQueue();
}

/*!
  \memberof engineActionLogic

  Queues the order to move fast; starts processing of the queue.
  */
function moveFastTo (newX, newY) {
    queueOrder("Move fast", newX, newY);
    processQueue();
}

/*!
  \memberof engineActionLogic

  Queues the order to move at slow speed; starts processing of the queue.
  */
function sneakTo (newX, newY) {
    queueOrder("Sneak", newX, newY);
    processQueue();
}

/*!
  \memberof engineActionLogic

  Performs movement order by starting the rotation animation.

  When rotation stops, code in Unit.qml picks up and starts movement.
  */
function performMovement (newX, newY, factor) {
    __tempX = newX - (centerX);
    __tempY = newY - (centerY);

    var newRotation = Logic.rotationAngle(x, y,
                                          __tempX,
                                          __tempY);
    rotationAnimation.duration = Logic.rotationDuration(rotation,
                                                        newRotation, rotationSpeed);
    rotationAnimation.to = newRotation;
    rotationAnimation.running = true;
    moving = true;

    var moveDuration = Logic.targetDistance(x, y,
                                            __tempX,
                                            __tempY) * 800 / (maxSpeed * factor);
    xMoveAnimation.duration = moveDuration;
    yMoveAnimation.duration = moveDuration;
}

/*!
  \memberof engineActionLogic

  Queues the order fire smoke with the turret; starts processing of the queue.
  */
function turretSmokeTo (targetX, targetY) {
    queueOrder("Smoke", targetX, targetY);
    processQueue();
}

/*!
  \memberof engineActionLogic

  Queues the order attack with the turret; starts processing of the queue.
  */
function turretFireTo (targetX, targetY) {
    queueOrder("Attack", targetX, targetY);
    processQueue();
}

/*!
  \memberof engineActionLogic

  Performs firing order by starting the turret rotation animation.

  When rotation stops, code in Tank.qml picks up and starts movement.
  */
function performTurretShooting (targetX, targetY) {
    __tempX = targetX;
    __tempY = targetY;
    var newRotation = Logic.rotationAngle(x, y,
                                          targetX - centerX,
                                          targetY - centerY) - rotation;
    turretRotationAnimation.duration = Logic.rotationDuration(turretRotation,
                                                              newRotation,
                                                              turretRotationSpeed);
    turretRotationAnimation.to = newRotation;
    turretRotationAnimation.running = true;

    changeStatus("ROTATING");
}

/*!
  \memberof engineActionLogic

  Cancels unit's orders, clears order queue.
  */
function cancelOrder () {
    changeStatus("STOPPED");
    clearOrderQueue();
    moving = false;

    if ((firing == false) && (smoking == false))  {
        xMoveAnimation.stop();
        yMoveAnimation.stop();
        rotationAnimation.stop();
        changeStatus("READY");
    }

    if ((firing == true) || (smoking == true))  {
        turretRotationAnimation.stop();
        smoking = false;
        firing = false;
        changeStatus("READY");
    }
}

/*!
  \memberof engineActionLogic

  Puts a new order at the end of a queue.
  */
function queueOrder (orderName, newX, newY) {
//    var component = Qt.createComponent("../../qml/units/Order.qml");
    // QRC is needed here, because files from different directories have acces
    // to this method, and relative paths don't work for all of them.
    var component = Qt.createComponent("qrc:/core/units/Order.qml");
    var order;

    if (component.status == Component.Ready) {
        order = component.createObject(root, {"x": newX, "y": newY, "operation": orderName});
    }

    var orderQueue = getOrderQueue();
    orderQueue.push(order);
}

/*!
  \memberof engineActionLogic

  Makes sure that queue in execution is not disturbed
  by new calls. Called to begin queue execution.
  */
function processQueue () {
    if (currentOrder == -1) {
        continueQueue();
    }
}

/*!
  \memberof engineActionLogic

  Processes next element in the queue.
  */
function continueQueue () {
    var orderQueue = getOrderQueue();
    var noOrdersLeft = true;

    for (var i = 0; i < orderQueue.length; i++) {
        var order = orderQueue[i];
        if (order.performed == true) {
            continue;
        } else {
            currentOrder = i;

            if (order.operation == "Move") {
                changeStatus("MOVING");
                performMovement(order.x, order.y, 1);
            } else if (order.operation == "Move fast") {
                changeStatus("MOVING FAST");
                performMovement(order.x, order.y, moveFastFactor);
            } else if (order.operation == "Sneak") {
                changeStatus("SNEAKING");
                performMovement(order.x, order.y, sneakFactor);
            } else if (order.operation == "Smoke") {
                performTurretShooting(order.x, order.y);
                smoking = true;
            } else if (order.operation == "Attack") {
                performTurretShooting(order.x, order.y);
                firing = true;
            }

            order.performed = true;
            noOrdersLeft = false;
            // Ensures that unit performs one order at a time
            break;
        }
    }

    if (noOrdersLeft == true) {
        clearOrderQueue();
    }
}

/*!
  \memberof engineActionLogic

  Calculates result of a hit
  */
function hit(byWhat, xWhere, yWhere) {
    // For now, not much logic is in ... :)
    cancelOrder();
    // Strangely, both state change calls invoke on base state only once!
//    if (states.contains("destroyed"))
        state = "destroyed";
//    else
        state = "destroyed_base";

    console.log("Hit! By: " + byWhat + ", where: (" + xWhere + ", " + yWhere + ")");
}
