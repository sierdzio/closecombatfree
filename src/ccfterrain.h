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

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSize>
#include <QImage>
#include <QPoint>
#include <QColor>
#include <QtCore/qmath.h>

#include "ccferror.h"

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

private:
    qreal targetDistance(qreal originX, qreal originY, qreal targetX, qreal targetY);
    QImage *terrainImage;
};

#endif // CCFTERRAIN_H
