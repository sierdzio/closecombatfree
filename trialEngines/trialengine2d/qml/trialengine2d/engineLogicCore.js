.pragma library

function rotationAngle(oldX, oldY, newX, newY) {
    var result = 0;

    if (newX == oldX) {
        if (newY > oldY)
            result = 0;
        else
            result = 180;
        return result;
    }
    else if (newY == oldY) {
        if (newX > oldX)
            result = 90;
        else
            result = 270;
        return result;
    }

    var angle = (Math.atan(Math.abs(newX - oldX) / Math.abs(newY - oldY)) * 180 / Math.PI);

    if ((newY > oldY) && (newX > oldX)) // 2. quarter
        result = 180 - angle;
    else if ((newY > oldY) && (newX < oldX)) // 3. quarter
        result = 180 + angle;
    else if ((newY < oldY) && (newX < oldX)) // 4. quarter
        result = 360 - angle;
    else // 1. quarter
        result = angle;

    return result;
}

function targetDistance(originX, originY, targetX, targetY) {
    var result = 0;

    if (targetX == originX) {
        result = Math.abs(originY - targetY);
        return result;
    }
    else if (targetY == originY) {
        result = Math.abs(originX - targetX);
        return result;
    }

    result = Math.sqrt(Math.pow((originX - targetX), 2) + Math.pow((originY - targetY), 2));

    return result;
}
