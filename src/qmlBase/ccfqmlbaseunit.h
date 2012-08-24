#ifndef CCFQMLBASEUNIT_H
#define CCFQMLBASEUNIT_H

#include <QtQuick/QQuickItem>
#include <QtQml/QQmlListReference>

/*!
 * \brief The CcfQmlBaseUnit class is an experiment to see if moving most functionality
 * from Unit.qml to here is sane, doable, and does not require too much effort
 * and nasty workarounds.
 */
class CcfQmlBaseUnit : public QQuickItem
{
    Q_OBJECT
public:
    explicit CcfQmlBaseUnit(QQuickItem *parent = 0);

    // Many of those can be changed into local vars, probalby
    Q_PROPERTY(QString objectType READ get WRITE set NOTIFY Changed)
    // Sadly, this is needed for file saving:
    Q_PROPERTY(QString unitFileName READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(QString unitType READ get WRITE set NOTIFY Changed)
    // TODO: Hange to QImage, perhaps?
    Q_PROPERTY(QString unitLogo READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(QString unitStatus READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(QString unitSide READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int groupNumber READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int unitIndex READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(bool sideMarkVisible READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(QString sideMarkSource READ get WRITE set NOTIFY Changed)
    /*! Defines a set of side marks to be used to decorate this unit.
        Side mark filenames should be made according to this template:
       <root>/img/units/sideMarks/sideMark_<sideMarkSet>_<sideNumber>.png
       */
    Q_PROPERTY(QString sideMarkSet READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int rotationSpeed READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int turretRotationSpeed READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int maxSpeed READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int acceleration READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int unitWidth READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int unitHeight READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(QQmlListReference soldiers READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(QQmlListReference orders READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(qreal moveFastFactor READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(qreal sneakFactor READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int centerX READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int centerY READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int __tempX READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int __tempY READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(QString scheduledOperation READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int currentOrder READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(bool selected READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(bool firing READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(bool smoking READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(int defenceSphereRotation READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(QString defenceSphereColor READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(bool paused READ get WRITE set NOTIFY Changed)
    Q_PROPERTY(bool moving READ get WRITE set NOTIFY Changed)
};

#endif // CCFQMLBASEUNIT_H
