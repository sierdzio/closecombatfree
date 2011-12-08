function moveTo (newX, newY) {
    changeStatus("MOVING");
    performMovement(newX, newY, 1);    
}

function moveFastTo (newX, newY) {
    changeStatus("MOVING FAST");
    performMovement(newX, newY, moveFastFactor);    
}

function sneakTo (newX, newY) {
    changeStatus("SNEAKING");
    performMovement(newX, newY, sneakFactor);    
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
    performTurretShooting(targetX, targetY);
    smoking = true;
}

function turretFireTo (targetX, targetY) {
    performTurretShooting(targetX, targetY);
    firing = true;
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
    turretRotationRoot.running = true;

    changeStatus("ROTATING");
}

function cancelOrder () {
    changeStatus("STOPPED");

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
