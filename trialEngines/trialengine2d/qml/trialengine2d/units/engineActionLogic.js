function moveTo (newX, newY) {
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
                                            __tempY) * 800 / (maxSpeed);
    xMoveAnimation.duration = moveDuration;
    yMoveAnimation.duration = moveDuration;
    changeStatus("MOVING");
}

function fireTo (targetX, targetY) {
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
    turret.__firing = true;
}
