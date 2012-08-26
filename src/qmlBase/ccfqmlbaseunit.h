#ifndef CCFQMLBASEUNIT_H
#define CCFQMLBASEUNIT_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtQuick/QQuickItem>
#include <QtQml/QQmlComponent>
//#include <QtQml/QQmlListReference>
//#include <QtCore/QVariantList>

/*!
  \defgroup CloseCombatFree Game code
  @{
 */

class CcfMain;

/*!
  The CcfQmlBaseUnit class is an experiment to see if moving most functionality
  from Unit.qml to here is sane, doable, and does not require too much effort
  and nasty workarounds.
 */
class CcfQmlBaseUnit : public QQuickItem
{
    Q_OBJECT

    // Many of those can be changed into local vars, probalby
    Q_PROPERTY(QString objectType READ getObjectType WRITE setObjectType NOTIFY objectTypeChanged)
    // Sadly, this is needed for file saving:
    Q_PROPERTY(QString unitFileName READ getUnitFileName WRITE setUnitFileName NOTIFY unitFileNameChanged)
    Q_PROPERTY(QString unitType READ getUnitType WRITE setUnitType NOTIFY unitTypeChanged)
    // TODO: Change to QImage, perhaps?
    Q_PROPERTY(QString unitLogo READ getUnitLogo WRITE setUnitLogo NOTIFY unitLogoChanged)
    Q_PROPERTY(QString unitStatus READ getUnitStatus WRITE setUnitStatus NOTIFY unitStatusChanged)
    Q_PROPERTY(QString unitSide READ getUnitSide WRITE setUnitSide NOTIFY unitSideChanged)
    Q_PROPERTY(int groupNumber READ getGroupNumber WRITE setGroupNumber NOTIFY groupNumberChanged)
    Q_PROPERTY(int unitIndex READ getUnitIndex WRITE setUnitIndex NOTIFY unitIndexChanged)
    Q_PROPERTY(bool sideMarkVisible READ getSideMarkVisible WRITE setSideMarkVisible NOTIFY sideMarkVisibleChanged)
    Q_PROPERTY(QString sideMarkSource READ getSideMarkSource WRITE setSideMarkSource NOTIFY sideMarkSourceChanged)
    /*! Defines a set of side marks to be used to decorate this unit.
        Side mark filenames should be made according to this template:
       <root>/img/units/sideMarks/sideMark_<sideMarkSet>_<sideNumber>.png
       */
    Q_PROPERTY(QString sideMarkSet READ getSideMarkSet WRITE setSideMarkSet NOTIFY sideMarkSetChanged)
    Q_PROPERTY(int rotationSpeed READ getRotationSpeed WRITE setRotationSpeed NOTIFY rotationSpeedChanged)
    Q_PROPERTY(int turretRotationSpeed READ getTurretRotationSpeed WRITE setTurretRotationSpeed NOTIFY turretRotationSpeedChanged)
    Q_PROPERTY(int maxSpeed READ getMaxSpeed WRITE setMaxSpeed NOTIFY maxSpeedChanged)
    Q_PROPERTY(int acceleration READ getAcceleration WRITE setAcceleration NOTIFY accelerationChanged)
    Q_PROPERTY(int unitWidth READ getUnitWidth WRITE setUnitWidth NOTIFY unitWidthChanged)
    Q_PROPERTY(int unitHeight READ getUnitHeight WRITE setUnitHeight NOTIFY unitHeightChanged)
//    Q_PROPERTY(QQmlListReference soldiers READ getSoldiers WRITE setSoldiers NOTIFY soldiersChanged)
    Q_PROPERTY(qreal moveFastFactor READ getMoveFastFactor WRITE setMoveFastFactor NOTIFY moveFastFactorChanged)
    Q_PROPERTY(qreal sneakFactor READ getSneakFactor WRITE setSneakFactor NOTIFY sneakFactorChanged)
    Q_PROPERTY(int centerX READ getCenterX WRITE setCenterX NOTIFY centerXChanged)
    Q_PROPERTY(int centerY READ getCenterY WRITE setCenterY NOTIFY centerYChanged)
    Q_PROPERTY(int tempX READ getTempX WRITE setTempX NOTIFY tempXChanged)
    Q_PROPERTY(int tempY READ getTempY WRITE setTempY NOTIFY tempYChanged)
    Q_PROPERTY(QString scheduledOperation READ getScheduledOperation WRITE setScheduledOperation NOTIFY scheduledOperationChanged)
    Q_PROPERTY(int currentOrder READ getCurrentOrder WRITE setCurrentOrder NOTIFY currentOrderChanged)
    Q_PROPERTY(bool selected READ getSelected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(bool firing READ getFiring WRITE setFiring NOTIFY firingChanged)
    Q_PROPERTY(bool smoking READ getSmoking WRITE setSmoking NOTIFY smokingChanged)
    Q_PROPERTY(int defenceSphereRotation READ getDefenceSphereRotation WRITE setDefenceSphereRotation NOTIFY defenceSphereRotationChanged)
    Q_PROPERTY(QString defenceSphereColor READ getDefenceSphereColor WRITE setDefenceSphereColor NOTIFY defenceSphereColorChanged)
    Q_PROPERTY(bool paused READ getPaused WRITE setPaused NOTIFY pausedChanged)
    Q_PROPERTY(bool moving READ getMoving WRITE setMoving NOTIFY movingChanged)

public:
    explicit CcfQmlBaseUnit(QQuickItem *parent = 0);

    Q_INVOKABLE QString operation(int index);
    Q_INVOKABLE void changeStatus(const QString &newStatusMessage);
    Q_INVOKABLE void performMovement(qreal newX, qreal newY, qreal factor);
    Q_INVOKABLE void performTurretShooting(qreal targetX, qreal targetY);
    Q_INVOKABLE void hit(QObject *byWhat, qreal xWhere, qreal yWhere);

    Q_INVOKABLE void continueQueue();
    Q_INVOKABLE void cancelOrder();
    Q_INVOKABLE void queueOrder(const QString &orderName, qreal x, qreal y, QObject *reparent);

    Q_INVOKABLE void moveTo(qreal newX, qreal newY, QObject *reparent);
    Q_INVOKABLE void moveFastTo(qreal newX, qreal newY, QObject *reparent);
    Q_INVOKABLE void sneakTo(qreal newX, qreal newY, QObject *reparent);
    Q_INVOKABLE void turretFireTo(qreal targetX, qreal targetY, QObject *reparent);
    Q_INVOKABLE void turretSmokeTo(qreal targetX, qreal targetY, QObject *reparent);

protected:
    QObject *createOrder();
    void processQueue();
    void clearOrderQueue();
    void deleteOrder(int index);

signals:
    void unitStatusChanged(const QString &newStatus, int index);
    void movementBegan();

private:
    CcfMain *m_mainInstance;
    QQmlComponent *m_ordersComponent;
    QList<QObject *> m_orders;

    // // // //
    // Everything below is property handling:
    // // // //
protected:
    // Property getters:
    QString getObjectType() const;
    QString getUnitFileName() const;
    QString getUnitType() const;
    QString getUnitLogo() const;
    QString getUnitStatus() const;
    QString getUnitSide() const;
    int getGroupNumber() const;
    int getUnitIndex() const;
    bool getSideMarkVisible() const;
    QString getSideMarkSource() const;
    QString getSideMarkSet() const;
    int getRotationSpeed() const;
    int getTurretRotationSpeed() const;
    int getMaxSpeed() const;
    int getAcceleration() const;
    int getUnitWidth() const;
    int getUnitHeight() const;
//    QQmlListReference getSoldiers();
    qreal getMoveFastFactor() const;
    qreal getSneakFactor() const;
    int getCenterX() const;
    int getCenterY() const;
    int getTempX() const;
    int getTempY() const;
    QString getScheduledOperation() const;
    int getCurrentOrder() const;
    bool getSelected() const;
    bool getFiring() const;
    bool getSmoking() const;
    int getDefenceSphereRotation() const;
    QString getDefenceSphereColor() const;
    bool getPaused() const;
    bool getMoving() const;

    // Property setters:
    void setObjectType(const QString &objectType);
    void setUnitFileName(const QString &unitFileName);
    void setUnitType(const QString &unitType);
    void setUnitLogo(const QString &unitLogo);
    void setUnitStatus(const QString &unitStatus);
    void setUnitSide(const QString &unitSide);
    void setGroupNumber(int groupNumber);
    void setUnitIndex(int unitIndex);
    void setSideMarkVisible(bool sideMarkVisible);
    void setSideMarkSource(const QString &sideMarkSource);
    void setSideMarkSet(const QString &sideMarkSet);
    void setRotationSpeed(int rotationSpeed);
    void setTurretRotationSpeed(int turretRotationSpeed);
    void setMaxSpeed(int maxSpeed);
    void setAcceleration(int acceleration);
    void setUnitWidth(int unitWidth);
    void setUnitHeight(int unitHeight);
//    void setSoldiers(const QQmlListReference &soldiers);
    void setMoveFastFactor(qreal moveFastFactor);
    void setSneakFactor(qreal sneakFactor);
    void setCenterX(int centerX);
    void setCenterY(int centerY);
    void setTempX(int tempX);
    void setTempY(int tempY);
    void setScheduledOperation(const QString &scheduledOperation);
    void setCurrentOrder(int currentOrder);
    void setSelected(bool selected);
    void setFiring(bool firing);
    void setSmoking(bool smoking);
    void setDefenceSphereRotation(int defenceSphereRotation);
    void setDefenceSphereColor(const QString &defenceSphereColor);
    void setPaused(bool paused);
    void setMoving(bool moving);

signals:
    void objectTypeChanged();
    void unitFileNameChanged();
    void unitTypeChanged();
    void unitLogoChanged();
    void unitStatusChanged();
    void unitSideChanged();
    void groupNumberChanged();
    void unitIndexChanged();
    void sideMarkVisibleChanged();
    void sideMarkSourceChanged();
    void sideMarkSetChanged();
    void rotationSpeedChanged();
    void turretRotationSpeedChanged();
    void maxSpeedChanged();
    void accelerationChanged();
    void unitWidthChanged();
    void unitHeightChanged();
    void soldiersChanged();
    void moveFastFactorChanged();
    void sneakFactorChanged();
    void centerXChanged();
    void centerYChanged();
    void tempXChanged();
    void tempYChanged();
    void scheduledOperationChanged();
    void currentOrderChanged();
    void selectedChanged(bool state, int index);
    void firingChanged();
    void smokingChanged();
    void defenceSphereRotationChanged();
    void defenceSphereColorChanged();
    void pausedChanged();
    void movingChanged();

private:
    // Properties:
    QString m_objectType;
    QString m_unitFileName;
    QString m_unitType;
    QString m_unitLogo;
    QString m_unitStatus;
    QString m_unitSide;
    int m_groupNumber;
    int m_unitIndex;
    bool m_sideMarkVisible;
    QString m_sideMarkSource;
    QString m_sideMarkSet;
    int m_rotationSpeed;
    int m_turretRotationSpeed;
    int m_maxSpeed;
    int m_acceleration;
    int m_unitWidth;
    int m_unitHeight;
//    QQmlListReference m_soldiers;
    qreal m_moveFastFactor;
    qreal m_sneakFactor;
    int m_centerX;
    int m_centerY;
    int m_tempX;
    int m_tempY;
    QString m_scheduledOperation;
    int m_currentOrder;
    bool m_selected;
    bool m_firing;
    bool m_smoking;
    int m_defenceSphereRotation;
    QString m_defenceSphereColor;
    bool m_paused;
    bool m_moving;
};

/*! @}*/

#endif // CCFQMLBASEUNIT_H
