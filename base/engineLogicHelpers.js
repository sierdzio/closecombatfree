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

function angleTo8Step(angle) {
    if ((angle <= 22) || (angle > 337)) {
        return 270;
    } else if (angle <= 67) {
        return 315;
    } else if (angle <= 112) {
        return 0;
    } else if (angle <= 157) {
        return 45;
    } else if (angle <= 202) {
        return 90;
    } else if (angle <= 247) {
        return 135;
    } else if (angle <= 292) {
        return 180;
    } else if (angle <= 337) {
        return 225;
    }
}

function rotationDuration(oldRotation, newRotation, rotationSpeed) {
    var tempNewRotation = newRotation;
    var tempOldRotation = oldRotation;
    var rotationChange = newRotation - oldRotation;

    if (oldRotation == tempNewRotation)
        return 0;

    if (tempOldRotation == 0)
        tempOldRotation = 360;

    if ((newRotation > 180) && (oldRotation < 180)) {
        rotationChange = tempNewRotation - tempOldRotation;
    } else if ((oldRotation > 180) && (newRotation < 180)) {
        rotationChange = tempOldRotation - tempNewRotation;
    }

    var dur = (rotationSpeed * Math.abs(rotationChange));
    return Math.round(dur);
}

function colorForOrder(orderName) {
    var result = "ERROR";

    if (orderName == "Move") {
        result = "#22ff22";
    } else if (orderName == "Move fast") {
        result = "#771b91";
    } else if (orderName == "Sneak") {
        result = "#f0dd0c";
    } else if (orderName == "Smoke") {
        result = "#ffa000";
    } else if (orderName == "Attack") {
        result = "#ff2222";
    }

    return result;
}

function colorForStatus(statusMessage) {
    var result = "ERROR";

    if (statusMessage == "READY") {
        result = "#115511";
    } else if (statusMessage == "STOPPED") {
        result = "#ee5511";
    } else if (statusMessage == "MOVING") {
        result = "#11aa11";
    } else if (statusMessage == "MOVING FAST") {
        result = "#11aa11";
    } else if (statusMessage == "SNEAKING") {
        result = "#11aa11";
    } else if (statusMessage == "AMBUSHING") {
        result = "#aaaaaa";
    } else if (statusMessage == "DEFENDING") {
        result = "#aaaaaa";
    } else if (statusMessage == "KIA") {
        result = "#000000";
    } else if (statusMessage == "DAMAGED") {
        result = "#aa1111";
    } else if (statusMessage == "WOUNDED") {
        result = "#555511";
    } else if (statusMessage == "INCAPACITATED") {
        result = "#885511";
    } else if (statusMessage == "ROTATING") {
        result = "#11aa11";
    } else if (statusMessage == "FIRING") {
        result = "#aa1111";
    }

    return result;
}
