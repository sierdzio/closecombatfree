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

#include <qmath.h>
#include <QSize>
#include <QPoint>
#include <QColor>
#include <QImage>

#include <QSGNode>
#include <QSGSimpleTextureNode>
#include <QSGTexture>

#include "ccfqmlbasemap.h"
#include "ccfmain.h"

CcfQmlBaseMap::CcfQmlBaseMap(QQuickItem *parent) :
    CcfObjectBase(parent)
{
    setWidth(0.0);
    setHeight(0.0);

    mBackgroundTexture = NULL;
    mHipsometricTexture = NULL;
    mHipsometricMapInFront = false;
    setHipsometricImage("img/maps/hipsometric_default.png");
    setPropOpacity(0.5);

    mMainInstance = CcfMain::instance();
}

/*!
  Swaps background image that is currently visible to the next one in line.
  */
void CcfQmlBaseMap::toggleBackgroundImage()
{
    mHipsometricMapInFront = !mHipsometricMapInFront;
    setFlags(QQuickItem::ItemHasContents);
    update();
}

QSGNode *CcfQmlBaseMap::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *update)
{
    Q_UNUSED(update);

    QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>(oldNode);

    if (!node) {
        node = new QSGSimpleTextureNode();
    }

    if (mBackgroundPath.isEmpty() || mHipsometricMapInFront) {
        mHipsometricTexture = window()->createTextureFromImage(mHipsometricImage);
        node->setTexture(mHipsometricTexture);
    } else {
        mBackgroundTexture = window()->createTextureFromImage(mBackgroundImage);
        node->setTexture(mBackgroundTexture);
    }

    node->setRect(boundingRect());
    return node;
}

QString CcfQmlBaseMap::getBackgroundImage() const
{
    return mBackgroundPath;
}

QString CcfQmlBaseMap::getHipsometricImage() const
{
    return mHipsometricPath;
}

qreal CcfQmlBaseMap::getPropOpacity() const
{
    return mPropOpacity;
}

void CcfQmlBaseMap::setBackgroundImage(const QString &path)
{
    if (mBackgroundPath == path)
        return;

    mBackgroundPath = path;

    if (path.isEmpty()) {
        mBackgroundTexture = NULL;
    } else {
        if (mBackgroundTexture)
            mBackgroundTexture->deleteLater();

        mBackgroundImage = QImage(path);
        setWidth(mBackgroundImage.width());
        setHeight(mBackgroundImage.height());
        setFlags(QQuickItem::ItemHasContents);
    }

    update();
    emit backgroundImageChanged();
}

void CcfQmlBaseMap::setHipsometricImage(const QString &path)
{
    if (mHipsometricPath == path)
        return;

    mHipsometricPath = path;

    if (path.isEmpty()) {
        mHipsometricTexture = NULL;
    } else {
        if (mHipsometricTexture)
            mHipsometricTexture->deleteLater();

        if (mBackgroundTexture) {
            mHipsometricImage = QImage(path).scaled(QSize(width(), height()));
        } else {
            mHipsometricImage = QImage(path);
        }
    }

    emit hipsometricImageChanged();
}

void CcfQmlBaseMap::setPropOpacity(qreal propOpacity)
{
    if (mPropOpacity == propOpacity)
        return;

    mPropOpacity = propOpacity;
    emit propOpacityChanged();
}

void CcfQmlBaseMap::setUnits(const QObjectList &units)
{
    for (int i = 0; i < units.length(); ++i) {
        if (units.at(i)->getString("objectType") == "unit") {
            connect(units.at(i), SIGNAL(positionChanged(qreal, qreal, int)),
                    this, SLOT(checkForHits(qreal, qreal, int)));
        }
    }
}

void CcfQmlBaseMap::checkForHits(qreal x, qreal y, int index)
{
    Q_UNUSED(index);

    if (!childExistsAt(x, y))
        return;

    QObject *child = childAt(x, y);
    // WARNING! This checks x and y only. No check for width/height/rotation. Yet.

    mlogger->log("Hit! Who: " + child->objectName());

    if (child->property("topVisible").isValid())
        invoke(child, "removeTop");
}

bool CcfQmlBaseMap::childExistsAt(qreal x, qreal y)
{
    QObject *child = childAt(x, y);
    if (!child)
        return false;
    else
        return true;
}

QObjectList CcfQmlBaseMap::getProps()
{
    return children();
}

QVariantMap CcfQmlBaseMap::terrainInfo(qreal x, qreal y)
{
    // This method is intended to extract terrain information on a given point.
    // This info should include: type of terrain/ obstacle/ prop, height over "0" level,
    // how much cover does a given spot give to a unit.
    QVariantMap result;

    int info = pixelInfo(x, y);
    result.insert("heightOverZero", info / 10);

    if (childExistsAt(x, y)) {
        QObject *child = childAt(x, y);

        result.insert("objectType", child->objectName());
        result.insert("cover", child->getString("cover"));
    } else {
        result.insert("objectType", "unknown");
        result.insert("cover", "poor");
    }

    return result;
}

QString CcfQmlBaseMap::terrainInfoString(qreal x, qreal y)
{
    QVariantMap info = terrainInfo(x, y);
    QString result = "Terrain: " + info.value("objectType").toString()
        + ". Height: " + info.value("heightOverZero").toString() + " meters"
        + ". Cover: " + info.value("cover").toString();
    return result;
}

/*!
  Returns pixel (height) information.

  For a given set of coordinates (\a x, \a y) returns a sum of pixel's
  colour values.
  */
int CcfQmlBaseMap::pixelInfo(qreal x, qreal y)
{
    QRgb result(mHipsometricImage.pixel(QPoint((int) x, (int) y)));
    return qRed(result) + qGreen(result) + qBlue(result);
}

/*!
  Detects obstacles on the aimline.

  Returns distance to the nearest obstacle.
  */
int CcfQmlBaseMap::checkForTerrainInLOS(qreal x1, qreal y1, qreal x2, qreal y2, QObject *currentUnit)
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
  Returns true if target is visible.
 */
bool CcfQmlBaseMap::isTargetVisible(qreal x1, qreal y1, qreal x2, qreal y2)
{
        qreal distance = targetDistance(x1, y1, x2, y2);
        qreal a = (y2 - y1) / (x2 - x1);
        qreal b = y1 - (a * x1);
        qreal x = x2;
        qreal y = y2;

        qreal originHeight = pixelInfo(x1, y1);
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
            qreal currentHeight = pixelInfo(x, y);
            if ((currentHeight > originHeight) && (currentHeight > targetHeight)) {
                return false;
            }
        }
        return true;
}

/*!
  Determines the distance between two points.
  */
qreal CcfQmlBaseMap::targetDistance(qreal originX, qreal originY, qreal targetX, qreal targetY)
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

/*!
  See if the coordinates are inside currently shown QImage.
  */
bool CcfQmlBaseMap::checkCoordinateValidity(qreal x, qreal y)
{
    if (!mBackgroundTexture && !mHipsometricTexture)
        return false; // Images are empty!

    if (x < 0 || x > mHipsometricImage.width())
        return false;

    if (y < 0 || y > mHipsometricImage.height())
        return false;

    return true;
}
