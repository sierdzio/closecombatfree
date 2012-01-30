#include "ccfterrain.h"

CcfTerrain::CcfTerrain(QObject *parent) :
    QObject(parent), CcfError()
{
}

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

int CcfTerrain::pixelInfo(int x, int y)
{
    QRgb result(terrainImage->pixel(QPoint(x, y)));
    return qRed(result) + qGreen(result) + qBlue(result);
}

int CcfTerrain::checkForTerrainInLOS(qreal x1, qreal y1, qreal x2, qreal y2, QObject *currentUnit)
{
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

qreal CcfTerrain::targetDistance(qreal originX, qreal originY, qreal targetX, qreal targetY)
{
    qreal result = 0;

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
