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

#include <QtCore/qmath.h>
#include <QtCore/QVariant>

#include "ccfenginehelpers.h"

/*!
   Standard constructor, takes in a \a parent, since it's a QObject.
 */
CcfEngineHelpers::CcfEngineHelpers(QObject *parent) :
    QObject(parent)
{
}

/*!
  Checks whether array contains a given object.

  Returns -1 if it does not, otherwise index of first hit is returned.
  */
int CcfEngineHelpers::arrayContains(QList<QObject *> array, QObject *objToCheck) {
//    for (int i = 0; i < array.length; ++i) {
//        if (array == objToCheck) {
//            return i;
//        }
//    }
//    return -1;
    return array.contains(objToCheck);
}

/*!
  Returns an angle between "up" and line drawn between 2 given points.
  */
qreal CcfEngineHelpers::rotationAngle(qreal oldX, qreal oldY,
                             qreal newX, qreal newY)
{
    qreal result = 0;

    if (qFuzzyCompare(newX, oldX)) {
        if (newY > oldY)
            result = 0;
        else
            result = 180;
        return result;
    }
    else if (qFuzzyCompare(newY, oldY)) {
        if (newX > oldX)
            result = 90;
        else
            result = 270;
        return result;
    }

    qreal angle = (qAtan(qAbs(newX - oldX) / qAbs(newY - oldY)) * 180 / 3.14159);

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
  Returns a distance between two given points.
  */
qreal CcfEngineHelpers::targetDistance(qreal originX, qreal originY,
                              qreal targetX, qreal targetY)
{
    qreal result = 0;

    if (qFuzzyCompare(targetX, originX)) {
        result = qAbs(originY - targetY);
        return result;
    } else if (qFuzzyCompare(targetY, originY)) {
        result = qAbs(originX - targetX);
        return result;
    }

    result = qSqrt(qPow((originX - targetX), 2) + qPow((originY - targetY), 2));

    return result;
}

/*!
  Converts any given angle (0 <=> 360) into one of 8 headings.

  This is used to position defense and ambush spheres.
  */
qreal CcfEngineHelpers::angleTo8Step(qreal angle) {
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

    return 0;
}

/*!
  Computes duration needed for rotation animation.
  */
int CcfEngineHelpers::rotationDuration(qreal oldRotation, qreal newRotation,
                                qreal rotationSpeed)
{
    qreal tempNewRotation = newRotation;
    qreal tempOldRotation = oldRotation;
    qreal rotationChange = newRotation - oldRotation;

    if (qFuzzyCompare(oldRotation, tempNewRotation))
        return 0;

    if (qFuzzyCompare(tempOldRotation, 0))
        tempOldRotation = 360;

    if ((newRotation > 180) && (oldRotation < 180)) {
        rotationChange = tempNewRotation - tempOldRotation;
    } else if ((oldRotation > 180) && (newRotation < 180)) {
        rotationChange = tempOldRotation - tempNewRotation;
    }

    qreal dur = (rotationSpeed * qAbs(rotationChange));
    return qRound(dur);
}

/*!
  Returns HTML colour scheme for a given order.

  This is used by aimLine in Scenario.qml.
  */
QString CcfEngineHelpers::colorForOrder(const QString &orderName) {
    QString result = "ERROR";

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
  Returns HTML colour scheme for a given status message.

  This is used to show unit's status in RosterMenu.
  */
QString CcfEngineHelpers::colorForStatus(const QString &statusMessage) {
    QString result = "ERROR";

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
  Experimental:
  \li 0 - means no obstruction.
  \li Positive values - mean invisible.
  \li Negative values - mean obscured.
  */
qreal CcfEngineHelpers::checkForObstaclesInLOS(QList<QObject *> items, qreal x1, qreal y1,
                                      qreal x2, qreal y2, QObject *currentUnit) {
    qreal result = 0.0;
    qreal distance = targetDistance(x1, y1, x2, y2);
    qreal a = (y2 - y1) / (x2 - x1);
    qreal b = y1 - (a * x1);
    qreal x = x2;
    qreal y = y2;

    for (int i = 0; i < distance; ++i) {
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

        for (int j = 0; j < items.length(); ++j) {
            QObject *item = items.at(j);//items[j];
            if ((item == currentUnit) || (item == NULL)) {
                continue;
            }

            if (((x <= item->property("x").toReal() + item->property("width").toReal())
                 && (x >= item->property("x").toReal()))
                    && ((y <= item->property("y").toReal() + item->property("height").toReal())
                        && (y >= item->property("y").toReal()))) {
                result = targetDistance(x1, y1, x, y);
                return result;
            }
        }
    }
    return result;
}
