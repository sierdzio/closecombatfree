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
