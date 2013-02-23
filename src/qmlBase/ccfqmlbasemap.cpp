#include "ccfqmlbasemap.h"

CcfQmlBaseMap::CcfQmlBaseMap(QObject *parent) :
    CcfObjectBase(parent)
{
    mHipsometricMapInFront = false;
    mHipsometricImage = "../../img/maps/hipsometric_default.png";
    mPropOpacity = 0.5;
}

/*!
  Swaps background image that is currently visible to the next one in line.

  // TODO: refactor and optimise to conserve memory!
  */
void CcfQmlBaseMap::toggleBackgroundImage()
{
    int z1 = hipsometricMap.z;
    int z2 = background.z;


    if (mHipsometricMapInFront) {
        hipsometricMap.z = z2;
    } else {
        background.z = z1;
    }

    mHipsometricMapInFront = !mHipsometricMapInFront;
}

void CcfQmlBaseMap::paint(QPainter *painter)
{
}

QString CcfQmlBaseMap::getBackgroundImage() const
{
    return mBackgroundImage;
}

QString CcfQmlBaseMap::getHipsometricImage() const
{
    return mHipsometricImage;
}

qreal CcfQmlBaseMap::getPropOpacity() const
{
    return mPropOpacity;
}

void CcfQmlBaseMap::setBackgroundImage(const QString &path)
{
    if (mBackgroundImage == path)
        return;

    mBackgroundImage = path;
    emit backgroundImageChanged();
}

void CcfQmlBaseMap::setHipsometricImage(const QString &path)
{
    if (mHipsometricImage == path)
        return;

    mHipsometricImage = path;
    emit hipsometricImageChanged();
}

void CcfQmlBaseMap::setPropOpacity(qreal propOpacity)
{
    if (mPropOpacity == propOpacity)
        return;

    mPropOpacity = propOpacity;
    emit propOpacityChanged();
}
