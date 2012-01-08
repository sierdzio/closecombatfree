function moveTo (newX, newY) {
    queueOrder("Move", newX, newY);
    processQueue();
}

function moveFastTo (newX, newY) {
    queueOrder("Move fast", newX, newY);
    processQueue();
}

function sneakTo (newX, newY) {
    queueOrder("Sneak", newX, newY);
    processQueue();
}

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

    var moveDuration = Logic.targetDistance(x, y,
                                            __tempX,
                                            __tempY) * 800 / (maxSpeed * factor);
    xMoveAnimation.duration = moveDuration;
    yMoveAnimation.duration = moveDuration;
}

function turretSmokeTo (targetX, targetY) {
    queueOrder("Smoke", targetX, targetY);
    processQueue();
}

function turretFireTo (targetX, targetY) {
    queueOrder("Attack", targetX, targetY);
    processQueue();
}

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

function cancelOrder () {
    changeStatus("STOPPED");
    clearOrderQueue();

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

// Puts a new order at the end of a queue.
function queueOrder (orderName, newX, newY) {
//    var component = Qt.createComponent("../../qml/units/Order.qml");
    var component = Qt.createComponent("qrc:/core/units/Order.qml");
    var order;

    if (component.status == Component.Ready) {
        order = component.createObject(root, {"x": newX, "y": newY, "operation": orderName});
    }

    var orderQueue = getOrderQueue();
    orderQueue.push(order);
}

// Makes sure that queue in execution is not disturbed
// by new calls.
function processQueue () {
    if (currentOrder == -1) {
        continueQueue();
    }
}

// Processes next element in the queue.
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

// Calculates result of a hit
function hit(byWhat, xWhere, yWhere) {
    // For now, not much logic is in ... :)
    cancelOrder();
    // Strangely, both state chang calls invoke on base state only once!
//    if (states.contains("destroyed"))
        state = "destroyed";
//    else
        state = "destroyed_base";

    console.log("Hit! By: " + byWhat + ", where: (" + xWhere + ", " + yWhere + ")");
}
