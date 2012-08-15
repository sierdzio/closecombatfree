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

#include "ccfterrain.h"

/*!
  A simple constructor. Initialises parent QObject and CcfError.
  */
CcfTerrain::CcfTerrain(QObject *parent) :
    QObject(parent), CcfError()
{
}

/*!
  Sets the url of terrain image to use.

  TODO:
  WARNING: this method may be the cause of some of the memory leaks in game.
  When you load multiple scenarios in a single game run, some memory stays
  reserved, even though it should be cleaned. Maybe the fact that we are
  not flushing QImage anywhere is the cause?

  Also, we seem to be keeping images twice in memory - one copy in QML,
  and another in C++. This HAS TO be changed.
  */
void CcfTerrain::setTerrainImageUrl(const QString &url, int width, int height)
{
//    qDebug() << url;
    // Hack for QRC support
    QString nUrl = url;
    if (url.mid(0, 3) == "qrc") {
        nUrl.remove(0, 3);
    } else {
        nUrl.remove(0, 6);
    }
//    qDebug() << nUrl;
    QImage tempImage(nUrl);
    terrainImage = new QImage(tempImage.scaled(QSize(width, height)));
}

/*!
  Returns pixel (height) information.

  For a given set of coordinates (\a x, \a y) returns a sum of pixel's
  colour values.
  */
int CcfTerrain::pixelInfo(int x, int y)
{
    QRgb result(terrainImage->pixel(QPoint(x, y)));
    return qRed(result) + qGreen(result) + qBlue(result);
}

/*!
  Detects obstacles on the aimline.

  Returns distance to the nearest obstacle.
  */
int CcfTerrain::checkForTerrainInLOS(qreal x1, qreal y1, qreal x2, qreal y2, QObject *currentUnit)
{
    Q_UNUSED(currentUnit);
//    int centerX = currentUnit->property("centerX").toInt();
//    int centerY = currentUnit->property("centerY").toInt();

    int result = 0;
    qreal distance = targetDistance(x1, y1, x2, y2);
    qreal a = (y2 - y1) / (x2 - x1);
    qreal b = y1 - (a * x1);
    qreal x = x2;
    qreal y = y2;
    // Will be needed to calculate too steep angles for firing
//    qreal originHeight = pixelInfo(x1, y1);
    qreal targetHeight = pixelInfo(x2, y2);

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

        // Detect height in this particular pixel.
        if (pixelInfo(x, y) > targetHeight) {
            result = targetDistance(x1, y1, x, y);
            return result;
        }
    }
    return result;
}

/*!
  Determines the distance between two points.
  */
qreal CcfTerrain::targetDistance(qreal originX, qreal originY, qreal targetX, qreal targetY)
{
    qreal result = 0.0;

    if (targetX == originX) {
        result = qAbs(originY - targetY);
        return result;
    } else if (targetY == originY) {
        result = qAbs(originX - targetX);
        return result;
    }

    result = qSqrt(qPow((originX - targetX), 2) + qPow((originY - targetY), 2));

    return result;
}
