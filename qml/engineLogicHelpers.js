.pragma library

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

  \class engineLogicHelpers

  JavaScript library containing several helper functions needed throughout
  game code.
  */

/*!
  \memberof engineLogicHelpers

  Checks whether array contains a given object.

  Returns -1 if it does not, otherwise index of first hit is returned.
  \endverbatim
  */
function arrayContains(array, objToCheck) {
    for (var i = 0; i < array.length; ++i) {
        if (array[i] == objToCheck) {
            return i;
        }
    }
    return -1;
}

/*!
  \memberof engineLogicHelpers

  Returns an angle between "up" and line drawn between 2 given points.
  */
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

/*!
  \memberof engineLogicHelpers

  Returns a distance between two given points.
  */
function targetDistance(originX, originY, targetX, targetY) {
    var result = 0;

    if (targetX == originX) {
        result = Math.abs(originY - targetY);
        return result;
    } else if (targetY == originY) {
        result = Math.abs(originX - targetX);
        return result;
    }

    result = Math.sqrt(Math.pow((originX - targetX), 2) + Math.pow((originY - targetY), 2));

    return result;
}

/*!
  \memberof engineLogicHelpers

  Converts any given angle (0 <=> 360) into one of 8 headings.

  This is used to position defense and ambush spheres.
  */
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

/*!
  \memberof engineLogicHelpers

  Computes duration needed for rotation animation.
  */
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

/*!
  \memberof engineLogicHelpers

  Returns HTML colour scheme for a given order.

  This is used by aimLine in Scenario.qml.
  */
function colorForOrder(orderName) {
    var result = "ERROR";

    if (orderName == "Move") {
        result = "#22ff22";
    } else if (orderName == "Move fast") {
        result = "#b75bd1";
    } else if (orderName == "Sneak") {
        result = "#f0dd0c";
    } else if (orderName == "Smoke") {
        result = "#ffa000";
    } else if (orderName == "Attack") {
        result = "#ff2222";
    }

    return result;
}

/*!
  \memberof engineLogicHelpers

  Returns HTML colour scheme for a given status message.

  This is used to show unit's status in RosterMenu.
  */
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
    } else if (statusMessage == "RELOADING") {
        result = "#11aa11";
    } else if (statusMessage == "FIRING") {
        result = "#aa1111";
    }

    return result;
}

/*!
  \memberof engineLogicHelpers

  Experimental:
  \li 0 - means no obstruction.
  \li Positive values - mean invisible.
  \li Negative values - mean obscured.
  */

function checkForObstaclesInLOS(items, x1, y1, x2, y2, currentUnit) {
    var result = 0;
    var distance = targetDistance(x1, y1, x2, y2);
    var a = (y2 - y1) / (x2 - x1);
    var b = y1 - (a * x1);
    var x = x2;
    var y = y2;

    for (var i = 0; i < distance; ++i) {
        if (x2 >= x1) {
            // Prevent overlenghtening
            if (x > x2)
                break;
            x = x1 + i;
        } else {
            // Prevent overlenghtening
            if (x < x2)
                break;
            x = x1 - i;
        }

        y = (a * x) + b;

        for (var j = 0; j < items.length; ++j) {
            var item = items[j];
            if ((item == currentUnit) || (item == undefined)) {
                continue;
            }

            if (((x <= item.x + item.width) && (x >= item.x))
                    && ((y <= item.y + item.height) && (y >= item.y))) {
                result = targetDistance(x1, y1, x, y);
                return result;
            }
        }
    }
    return result;
}
