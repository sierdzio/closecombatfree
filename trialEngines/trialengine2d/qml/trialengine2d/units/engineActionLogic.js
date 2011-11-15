//var scheduledOperation;

function moveTo (newX, newY) {
    performMovement(newX, newY, 1);
    changeStatus("MOVING");
}

function moveFastTo (newX, newY) {
    performMovement(newX, newY, moveFastFactor);
    changeStatus("MOVING FAST");
}

function sneakTo (newX, newY) {
    performMovement(newX, newY, sneakFactor);
    changeStatus("SNEAKING");
}

function performMovement (newX, newY, factor) {
    __tempX = newX - (centerX);
    __tempY = newY - (centerY);

    var newRotation = Logic.rotationAngle(x, y,
                                          __tempX,
                                          __tempY);
    rotationAnimation.duration = Logic.rotationDuration(rotation,
                                                        newRotation, rotationSpeed);
    rotation = newRotation;

    var moveDuration = Logic.targetDistance(x, y,
                                            __tempX,
                                            __tempY) * 800 / (maxSpeed * factor);
    xMoveAnimation.duration = moveDuration;
    yMoveAnimation.duration = moveDuration;
}

function turretSmokeTo (targetX, targetY) {
    performTurretShooting(targetX, targetY);
    turret.__smoking = true;
}

function turretFireTo (targetX, targetY) {
    performTurretShooting(targetX, targetY);
    turret.__firing = true;
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
    turretRotation = newRotation;
    changeStatus("ROTATING");
}
