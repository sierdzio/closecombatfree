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

#ifndef CCFTERRAIN_H
#define CCFTERRAIN_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSize>
#include <QtCore/QPoint>

#include <QtGui/QImage>
#include <QtGui/QColor>

#include "ccferror.h"

/*!
  \ingroup CloseCombatFree
  @{
 */

/*!
  Class used in QML to get terrain information, mostly from textures.

  Application uses hipsometric map for terrain height. Value ("height")
  is calculated by ADDING int values (0-255) of red, green and blue components.
  This is then converted into meters by dividing by 10. This means,
  that the maximum height available in game is 76.5 meters.
  Should that not be enough, a different algorithm can easily be added
  ot substituted.
  */
class CcfTerrain : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit CcfTerrain(QObject *parent = 0);
    Q_INVOKABLE void setTerrainImageUrl(const QString &url, int width, int height);
    Q_INVOKABLE int pixelInfo(int x, int y);
    Q_INVOKABLE int checkForTerrainInLOS(qreal x1, qreal y1,
                                         qreal x2, qreal y2,
                                         QObject *currentUnit);
    Q_INVOKABLE bool isTargetVisible(qreal x1, qreal y1, qreal x2, qreal y2);

private:
    qreal targetDistance(qreal originX, qreal originY, qreal targetX, qreal targetY);
    QImage *terrainImage;
};

/*! @}*/

#endif // CCFTERRAIN_H
