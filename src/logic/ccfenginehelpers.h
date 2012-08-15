#ifndef CCFENGINEHELPERS_H
#define CCFENGINEHELPERS_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>

class CcfEngineHelpers : public QObject
{
    Q_OBJECT
public:
    explicit CcfEngineHelpers(QObject *parent = 0);

    Q_INVOKABLE int arrayContains(QList<QObject *> array, QObject *objToCheck);
    Q_INVOKABLE qreal rotationAngle(qreal oldX, qreal oldY, qreal newX, qreal newY);
    Q_INVOKABLE qreal targetDistance(qreal originX, qreal originY, qreal targetX, qreal targetY);
    Q_INVOKABLE qreal angleTo8Step(qreal angle);
    Q_INVOKABLE int rotationDuration(qreal oldRotation, qreal newRotation, qreal rotationSpeed);
    Q_INVOKABLE QString colorForOrder(const QString &orderName);
    Q_INVOKABLE QString colorForStatus(const QString &statusMessage);
    Q_INVOKABLE qreal checkForObstaclesInLOS(QList<QObject *> items, qreal x1, qreal y1,
                           qreal x2, qreal y2, QObject *currentUnit);
};

#endif // CCFENGINEHELPERS_H
