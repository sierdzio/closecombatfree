/****************************************************************************
** Close Combat Free is a free, easily moddable CC-like game.
** Copyright (C) 2013 Tomasz Siekierda
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

#ifndef CCFQMLBASEMAP_H
#define CCFQMLBASEMAP_H

#include <QObject>
#include <QImage>
#include <QVariantMap>
#include <QQuickItem>
#include <QQuickPaintedItem>

#include "logic/ccfobjectbase.h"

/*!
  \ingroup CloseCombatFree
  @{
 */

class QImage;
class QPainter;
class CcfMain;

/*!
  Base class for in-game Map objects.

  Class used in QML to get terrain information, mostly from textures.

  Application uses hipsometric map for terrain height. Value ("height")
  is calculated by ADDING int values (0-255) of red, green and blue components.
  This is then converted into meters by dividing by 10. This means,
  that the maximum height available in game is 76.5 meters.
  Should that not be enough, a different algorithm can easily be added
  ot substituted.
  */
class CcfQmlBaseMap : public QQuickPaintedItem
{
    Q_OBJECT
public:

    /*!
      Stores background image. Preferably in PNG, although all
      QML formats are supported.
      */
    Q_PROPERTY(QString backgroundImage READ getBackgroundImage WRITE setBackgroundImage NOTIFY backgroundImageChanged)

    /*!
      Holds path to hipsometric map of the terrain.
      Terrain defined by intensity of color in the image.
      Should be the same size as backgroundImage. If it's not,
      it will be stretched to match background.
      */
    Q_PROPERTY(QString hipsometricImage READ getHipsometricImage WRITE setHipsometricImage NOTIFY hipsometricImageChanged)

    /*!
      Defines global opacity value.
      Useful for temporarily revealing hidden props
      (like, water objects etc.)
      */
    Q_PROPERTY(qreal propOpacity READ getPropOpacity WRITE setPropOpacity NOTIFY propOpacityChanged)

    explicit CcfQmlBaseMap(QQuickItem *parent = 0);
    void toggleBackgroundImage();
    void paint(QPainter *painter);

    // Terrain info:
    Q_INVOKABLE int pixelInfo(qreal x, qreal y);
    Q_INVOKABLE int checkForTerrainInLOS(qreal x1, qreal y1,
                                         qreal x2, qreal y2,
                                         QObject *currentUnit);
    Q_INVOKABLE bool isTargetVisible(qreal x1, qreal y1, qreal x2, qreal y2);

    Q_INVOKABLE void setUnits(const QObjectList &units);
    Q_INVOKABLE bool childExistsAt(qreal x, qreal y);
    Q_INVOKABLE QObjectList getProps();
    Q_INVOKABLE QVariantMap terrainInfo(qreal x, qreal y);
    Q_INVOKABLE QString terrainInfoString(qreal x, qreal y);

signals:
    void backgroundImageChanged() const;
    void hipsometricImageChanged() const;
    void propOpacityChanged() const;

public slots:
    QString getBackgroundImage() const;
    QString getHipsometricImage() const;
    qreal getPropOpacity() const;

    void setBackgroundImage(const QString &path);
    void setHipsometricImage(const QString &path);
    void setPropOpacity(qreal propOpacity);
    void checkForHits(qreal x, qreal y, int index);

private:
    qreal targetDistance(qreal originX, qreal originY, qreal targetX, qreal targetY);
    bool checkCoordinateValidity(qreal x, qreal y);

    bool mHipsometricMapInFront;
    QString mBackgroundPath;
    QString mHipsometricPath;
    qreal mPropOpacity;
    QImage mBackgroundImage;
    QImage mHipsometricImage;
    QObjectList mUnits;
    CcfMain *mMainInstance;
};

/*! @}*/

#endif // CCFQMLBASEMAP_H
