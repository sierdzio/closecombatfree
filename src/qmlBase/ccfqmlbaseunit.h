#ifndef CCFQMLBASEUNIT_H
#define CCFQMLBASEUNIT_H

#include <QObject>
#include <QList>
#include <QVariant>
#include <QPoint>
#include <QQuickItem>
#include <QQmlListProperty>

#include "logic/ccfobjectbase.h"
#include "qmlBase/ccfqmlbasesoldier.h"

/*!
  \defgroup CloseCombatFree Game code
  @{
 */

class CcfMain;
class QQmlComponent;

/*!
  Convenience typedef that shotens the declaration of a list of Units. Much like
  QObjectList in that respect. See also CcfBaseObject::objectToUnitList().
  */
typedef QList<CcfQmlBaseUnit *> CcfUnitList;

/*!
  The CcfQmlBaseUnit class is an experiment to see if moving most functionality
  from Unit.qml to here is sane, doable, and does not require too much effort
  and nasty workarounds.
 */
class CcfQmlBaseUnit : public CcfObjectBase
{
    Q_OBJECT

public:
    /*!
      Stores type of the object (like "unit", or "unit/tank").
      */
    Q_PROPERTY(QString objectType READ getObjectType WRITE setObjectType NOTIFY objectTypeChanged)

    /*!
      This should hold the actual file name of a unit. This is not a nice solution,
      but allows to easily tell saving engine how to create a save file.

      Extension is not needed. Example entry: "Tank_tst1"
      */
    Q_PROPERTY(QString unitFileName READ getUnitFileName WRITE setUnitFileName NOTIFY unitFileNameChanged)

    /*!
      Holds unit's type. This is different to objectType in that here we already know this IS a unit.

      This is here to make a distinction between a tank, a gun etc.

      TODO: Probably those 2 properties can be merged!
      */
    Q_PROPERTY(QString unitType READ getUnitType WRITE setUnitType NOTIFY unitTypeChanged)

    /*!
      Stores a path to unit's logo (which is displayed in roster menu).

      TODO: Change to QImage, perhaps?
      */
    Q_PROPERTY(QString unitLogo READ getUnitLogo WRITE setUnitLogo NOTIFY unitLogoChanged)

    /*!
      Current unit's status.

      TODO: should it be combined with state?
      */
    Q_PROPERTY(QString unitStatus READ getUnitStatus WRITE setUnitStatus NOTIFY unitStatusChanged)

    /*!
      Side this unit is on (defaults to neutral).
      */
    Q_PROPERTY(QString unitSide READ getUnitSide WRITE setUnitSide NOTIFY unitSideChanged)

    /*!
      Stores unit's group number. Units that are in the same group can be selected at once
      by pressing relevant digit on the keyboard (0-9).

      For now, a single unit can be a member of only one group. This might change in the future,
      or it might stay as it is.
      */
    Q_PROPERTY(int groupNumber READ getGroupNumber WRITE setGroupNumber NOTIFY groupNumberChanged)

    /*!
      Unit's index. Needed by the engine.

      TODO: consider dropping this in favor of BaseScenario's index.
      */
    Q_PROPERTY(int unitIndex READ getUnitIndex WRITE setUnitIndex NOTIFY unitIndexChanged)

    /*!
      Controlls visibility of the side mark. Side mark is a small image showing unit's side allegiance.

      Useful in debugging and testing, and is currently true by default. For release, this will probably be
      turned off.
      */
    Q_PROPERTY(bool sideMarkVisible READ getSideMarkVisible WRITE setSideMarkVisible NOTIFY sideMarkVisibleChanged)

    /*!
      Holds the path to the image to be used as side mark.
      */
    Q_PROPERTY(QString sideMarkSource READ getSideMarkSource WRITE setSideMarkSource NOTIFY sideMarkSourceChanged)

    /*!
      Defines a set of side marks to be used to decorate this unit.
      Side mark filenames should be made according to this template:
      "root"/img/units/sideMarks/sideMark_sideMarkSet_sideNumber.png
       */
    Q_PROPERTY(QString sideMarkSet READ getSideMarkSet WRITE setSideMarkSet NOTIFY sideMarkSetChanged)

    /*!
      Speed with which this unit is able to rotate. Units are not clearly specified yet, this will come in later.

      Currently not used much, but will be vital later.
      */
    Q_PROPERTY(int rotationSpeed READ getRotationSpeed WRITE setRotationSpeed NOTIFY rotationSpeedChanged)

    /*!
      For tanks - the speed with which turret is rotating (relative to the hull, not the ground!).
      */
    Q_PROPERTY(int turretRotationSpeed READ getTurretRotationSpeed WRITE setTurretRotationSpeed NOTIFY turretRotationSpeedChanged)

    /*!
      Maximum speed this unit can move at. Not used at all, currently.

      For now, unit movement is controlled solely by QML animations. This is not good, and can be seen to work badly
      for long move orders.
      */
    Q_PROPERTY(int maxSpeed READ getMaxSpeed WRITE setMaxSpeed NOTIFY maxSpeedChanged)

    /*!
      Acceleration on the unit. Not used, currently.

      \sa maxSpeed
      */
    Q_PROPERTY(int acceleration READ getAcceleration WRITE setAcceleration NOTIFY accelerationChanged)

    /*!
      Holds the width of a unit. This is not always synonymous to "width" property, because some units need to
      have different sizing (for example in tanks, only the hull is taken into account when calculating hits. Lenght
      of the turret has no influence on that).
      */
    Q_PROPERTY(int unitWidth READ getUnitWidth WRITE setUnitWidth NOTIFY unitWidthChanged)

    /*!
      Holds the height of a unit. This is 2D height, so it acctually means "length" of the unit.

      \sa unitWidth
      */
    Q_PROPERTY(int unitHeight READ getUnitHeight WRITE setUnitHeight NOTIFY unitHeightChanged)

    /*!
      Moving speed of a unit is multiplied by that number, when it's moving fast.

      In contrast to original CC, this affects vehicles, too.
      */
    Q_PROPERTY(qreal moveFastFactor READ getMoveFastFactor WRITE setMoveFastFactor NOTIFY moveFastFactorChanged)

    /*!
      Moving speed of a unit is miltiplied by that number, when it's moving fast.

      In contrast to original CC, this affects vehicles, too.
      */
    Q_PROPERTY(qreal sneakFactor READ getSneakFactor WRITE setSneakFactor NOTIFY sneakFactorChanged)

    /*!
      Holds the central x coordinate of the unit. It's important because (0, 0) in QML means top-left corner, but
      for unit shooting and movement, we need to pivot transformations on the central point.

      TODO: merge those into a single QPoint property.

      \sa centerY
      */
    Q_PROPERTY(int centerX READ getCenterX WRITE setCenterX NOTIFY centerXChanged)

    /*!
      Holds the central y coordinate of the unit. It's important because (0, 0) in QML means top-left corner, but
      for unit shooting and movement, we need to pivot transformations on the central point.

      TODO: merge those into a single QPoint property.

      \sa centerX
      */
    Q_PROPERTY(int centerY READ getCenterY WRITE setCenterY NOTIFY centerYChanged)

    /*!
      Holds the index of the current order.

      TODO: This should also be thought over. Maybe it would be possible to remove this one, too.
      */
    Q_PROPERTY(int currentOrder READ getCurrentOrder WRITE setCurrentOrder NOTIFY currentOrderChanged)

    /*!
      Returns true if unit is selected. Set to true to select the unit.
      */
    Q_PROPERTY(bool selected READ getSelected WRITE setSelected NOTIFY selectedChanged)

    /*!
      Is true when unit is firing. This is mostly for internal use.

      TODO: rethink, try to remove.
      */
    Q_PROPERTY(bool firing READ getFiring WRITE setFiring NOTIFY firingChanged)

    /*!
      Is true when unit is smoking. This is mostly for internal use.

      TODO: rethink, try to remove.
      */
    Q_PROPERTY(bool smoking READ getSmoking WRITE setSmoking NOTIFY smokingChanged)

    /*!
      Holds the angle at which the defence sphere is shown.
      */
    Q_PROPERTY(int defenceSphereRotation READ getDefenceSphereRotation WRITE setDefenceSphereRotation NOTIFY defenceSphereRotationChanged)

    /*!
      Controls the colour of the defence sphere.
      */
    Q_PROPERTY(QString defenceSphereColor READ getDefenceSphereColor WRITE setDefenceSphereColor NOTIFY defenceSphereColorChanged)

    /*!
      Controlls whether this unit is paused.

      When user pauses the game, a signal is sent to all units, to trigger them into pause.

      As a side note it's worth saying that this distributed pause can be used to unpause a certain unit
      while the rest remains paused.
      */
    Q_PROPERTY(bool paused READ getPaused WRITE setPaused NOTIFY pausedChanged)

    /*!
      Is true when unit is moving.

      TODO: hide or remove.
      */
    Q_PROPERTY(bool moving READ getMoving WRITE setMoving NOTIFY movingChanged)

    explicit CcfQmlBaseUnit(QQuickItem *parent = 0);

    Q_INVOKABLE QString operation(int index = -1) const;
    Q_INVOKABLE QPoint orderTarget(int index = -1) const;
    Q_INVOKABLE void changeStatus(const QString &newStatusMessage);
    Q_INVOKABLE void performMovement(qreal newX, qreal newY, qreal factor);
    Q_INVOKABLE void performTurretShooting(qreal targetX, qreal targetY);
    Q_INVOKABLE void hit(QObject *byWhat, qreal xWhere, qreal yWhere);

    Q_INVOKABLE void continueQueue();
    Q_INVOKABLE void cancelOrder();
    Q_INVOKABLE void queueOrder(const QString &orderName, qreal x, qreal y, QObject *reparent = 0);

    Q_INVOKABLE void moveTo(qreal newX, qreal newY, QObject *reparent = 0);
    Q_INVOKABLE void moveFastTo(qreal newX, qreal newY, QObject *reparent = 0);
    Q_INVOKABLE void sneakTo(qreal newX, qreal newY, QObject *reparent = 0);
    Q_INVOKABLE void fireTo(qreal targetX, qreal targetY, QObject *reparent = 0);
    Q_INVOKABLE void smokeTo(qreal targetX, qreal targetY, QObject *reparent = 0);

    Q_INVOKABLE QVariantList soldiers();

public slots:
    void togglePause();

protected:
    QObject *createOrder(QObject *parent);
    void processQueue();
    void clearOrderQueue();
    void deleteOrder(int index);

signals:
    /*!
      Emitted when unit's status is changed.
      */
    void unitStatusChanged(const QString &newStatus, int unitIndex);

    /*!
      Emitted when the state of unit's movement is changed.
      */
    void movementStateChange(bool movingState, int unitIndex);

    /*!
      Emitted when an unit finishes an action.
      */
    void actionFinished (int index, qreal targetX, qreal targetY);

    /*!
      Emitted when unit begins moving.
      */
    void movementBegan();

private:
    bool isOrderIndexValid(int index) const;

    CcfMain *mMainInstance;
    QQmlComponent *mOrdersComponent;
    QObjectList mOrders;
    QVariantList mSoldiers;

    // Everything below is property handling:
public:
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
    qreal getMoveFastFactor() const;
    qreal getSneakFactor() const;
    int getCenterX() const;
    int getCenterY() const;
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
    void setMoveFastFactor(qreal moveFastFactor);
    void setSneakFactor(qreal sneakFactor);
    void setCenterX(int centerX);
    void setCenterY(int centerY);
    void setCurrentOrder(int currentOrder);
    void setSelected(bool selected);
    void setFiring(bool firing);
    void setSmoking(bool smoking);
    void setDefenceSphereRotation(int defenceSphereRotation);
    void setDefenceSphereColor(const QString &defenceSphereColor);
    void setPaused(bool paused);
    void setMoving(bool moving);

signals:
    /*!
      Emitted when object type is changed.
      */
    void objectTypeChanged();

    /*!
      Emitted when unit file name is changed.
      */
    void unitFileNameChanged();

    /*!
      Emitted when unit type is changed.
      */
    void unitTypeChanged();

    /*!
      Emitted when unit logo is changed.
      */
    void unitLogoChanged();

    /*!
      Emitted when unit status is changed.
      */
    void unitStatusChanged();

    /*!
      Emitted when unit side is changed.
      */
    void unitSideChanged();

    /*!
      Emitted when unit's group number is changed.
      */
    void groupNumberChanged();

    /*!
      Emitted when unit's index is changed.
      */
    void unitIndexChanged();

    /*!
      Emitted when unit's side mark's visibility is changed.
      */
    void sideMarkVisibleChanged();

    /*!
      Emitted when unit's side mark source path is changed.
      */
    void sideMarkSourceChanged();

    /*!
      Emitted when unit's set of side marks is changed.
      */
    void sideMarkSetChanged();

    /*!
      Emitted when unit's rotation speed is changed.
      */
    void rotationSpeedChanged();

    /*!
      Emitted when turret rotation speed is changed.
      */
    void turretRotationSpeedChanged();

    /*!
      Emitted when max speed is changed.
      */
    void maxSpeedChanged();

    /*!
      Emitted when acceleration is changed.
      */
    void accelerationChanged();

    /*!
      Emitted when unit's width is changed.
      */
    void unitWidthChanged();

    /*!
      Emitted when unit's height is changed.
      */
    void unitHeightChanged();

    /*!
      Emitted when move fast speed factor is changed.
      */
    void moveFastFactorChanged();

    /*!
      Emitted when sneak speed factor is changed.
      */
    void sneakFactorChanged();

    /*!
      Emitted when center x is changed.
      */
    void centerXChanged();

    /*!
      Emitted when center y is changed.
      */
    void centerYChanged();

    /*!
      Emitted when current order is changed.
      */
    void currentOrderChanged();

    /*!
      Emitted when selection state is changed.
      */
    void selectedChanged(bool state, int index);

    /*!
      Emitted when firing state is changed.
      */
    void firingChanged();

    /*!
      Emitted when smoking state is changed.
      */
    void smokingChanged();

    /*!
      Emitted when defence sphere's rotation is changed.
      */
    void defenceSphereRotationChanged();

    /*!
      Emitted when defence sphere's colour is changed.
      */
    void defenceSphereColorChanged();

    /*!
      Emitted when paused state is changed.
      */
    void pausedChanged();

    /*!
      Emitted when moving state is changed.
      */
    void movingChanged();

private:
    // Properties:
    QString mObjectType;
    QString mUnitFileName;
    QString mUnitType;
    QString mUnitLogo;
    QString mUnitStatus;
    QString mUnitSide;
    int mGroupNumber;
    int mUnitIndex;
    bool mSideMarkVisible;
    QString mSideMarkSource;
    QString mSideMarkSet;
    int mRotationSpeed;
    int mTurretRotationSpeed;
    int mMaxSpeed;
    int mAcceleration;
    int mUnitWidth;
    int mUnitHeight;
    qreal mMoveFastFactor;
    qreal mSneakFactor;
    int mCenterX;
    int mCenterY;
    int mCurrentOrder;
    bool mSelected;
    bool mFiring;
    bool mSmoking;
    int mDefenceSphereRotation;
    QString mDefenceSphereColor;
    bool mPaused;
    bool mMoving;
};

/*! @}*/

#endif // CCFQMLBASEUNIT_H
