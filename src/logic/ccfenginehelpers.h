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

#ifndef CCFENGINEHELPERS_H
#define CCFENGINEHELPERS_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>

/*!
  \defgroup CloseCombatFree Game code
  @{
 */

/*!
  The CcfEngineHelpers class contains various helper functions frequently needed
  by the engine.

  It is referenced in QML as EngineHelpers. It contains a lot of geometrical stuff
  + colour management + some minor things lacing in JavaScript.
 */
class CcfEngineHelpers : public QObject
{
    Q_OBJECT
public:
    explicit CcfEngineHelpers(QObject *parent = 0);

    Q_INVOKABLE static int arrayContains(QList<QObject *> array, QObject *objToCheck);
    Q_INVOKABLE static qreal rotationAngle(qreal oldX, qreal oldY, qreal newX, qreal newY);
    Q_INVOKABLE static qreal targetDistance(qreal originX, qreal originY, qreal targetX, qreal targetY);
    Q_INVOKABLE static qreal angleTo8Step(qreal angle);
    Q_INVOKABLE static int rotationDuration(qreal oldRotation, qreal newRotation, qreal rotationSpeed);
    Q_INVOKABLE static QString colorForOrder(const QString &orderName);
    Q_INVOKABLE static QString colorForStatus(const QString &statusMessage);
    Q_INVOKABLE static qreal checkForObstaclesInLOS(QList<QObject *> items, qreal x1, qreal y1,
                                             qreal x2, qreal y2, QObject *currentUnit);
    Q_INVOKABLE static bool isObstacleInLOS(QList<QObject *> items, qreal x1, qreal y1,
                                     qreal x2, qreal y2, QObject *currentUnit);
};

/*! @}*/

#endif // CCFENGINEHELPERS_H
