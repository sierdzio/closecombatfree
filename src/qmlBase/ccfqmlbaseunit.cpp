#include "ccfqmlbaseunit.h"
#include "ccfmain.h"
#include "logic/ccfenginehelpers.h"

#include <QQmlComponent>

/*!
  BaseUnit constructor - initialises all properties with default values,
  prepares orderMarker component for object generation.
  */
CcfQmlBaseUnit::CcfQmlBaseUnit(QQuickItem *parent) : CcfObjectBase(parent)
{
    mObjectType = QStringLiteral("unit");
    mUnitFileName = QStringLiteral("Unit");
    mUnitType = QStringLiteral("Generic unit");
    mUnitLogo = QStringLiteral("../../img/units/generic_unit_logo.png");
    mUnitStatus = QStringLiteral("READY");
    mUnitSide = QStringLiteral("neutral");
    mGroupNumber = 0;
    mUnitIndex = -1;
    mSideMarkVisible = true;
    mSideMarkSource = QStringLiteral("../../img/units/sideMarks/sideMark_side_1.png");
    mSideMarkSet = QStringLiteral("side");
    mRotationSpeed = 1;
    mTurretRotationSpeed = 1;
    mMaxSpeed = 1;
    mAcceleration = 1;
    mUnitWidth = 1;
    mUnitHeight = 1;
    mMoveFastFactor = 1.5;
    mSneakFactor = 0.4;
    mCurrentOrder = -1;
    mSelected = false;
    mFiring = false;
    mSmoking = false;
    mDefenceSphereRotation = 0;
    mPaused = false;
    mMoving = false;

    mMainInstance = CcfMain::instance();
    mOrdersComponent = new QQmlComponent(mMainInstance->engine(),
                                          QUrl::fromLocalFile("qml/gui/OrderMarker.qml"));
}

/*!
  Returns the name of the operation associated with order \a index, or current order
  if no index is given.
  */
QString CcfQmlBaseUnit::operation(int index) const
{
    if (index == -1) {
        if (mCurrentOrder != -1)
            index = mCurrentOrder;
        else
            index = 0;
    }

    if (isOrderIndexValid(index)) {
        return mOrders.at(index)->getString("operation");
    } else {
        mmain->logger()->error("Invalid unit operation index requested",
                             "Mighty Boosh is a great series");
        return QString();
    }
}

/*!
  Returns a destination point of order specified by \a index, or if none is given
  - the current order.
  */
QPoint CcfQmlBaseUnit::orderTarget(int index) const
{
    if (index == -1)
        index = mCurrentOrder;

    int x = 0;
    int y = 0;
    if (isOrderIndexValid(index)) {
        x = mOrders.at(index)->getInt("targetX");
        y = mOrders.at(index)->getInt("targetY");
    }

    return QPoint(x, y);
}

/*!
  Changes unit's status.
  */
void CcfQmlBaseUnit::changeStatus(const QString &newStatusMessage)
{
    mUnitStatus = newStatusMessage;
    emit unitStatusChanged(newStatusMessage, mUnitIndex);
}

/*!
  Performs movement order by starting the rotation animation.

  When rotation stops, code in Unit.qml picks up and starts movement.
  */
void CcfQmlBaseUnit::performMovement(qreal newX, qreal newY, qreal factor)
{
    qreal tempX = newX - (mCenterX);
    qreal tempY = newY - (mCenterY);

    qreal newRotation = CcfEngineHelpers::rotationAngle(x(), y(), tempX, tempY);
    QObject *rotationAnimation = child("rotationAnimation");
    if (rotationAnimation) {
        rotationAnimation->set("duration",
                               CcfEngineHelpers::rotationDuration(rotation(),
                                                                  newRotation, mRotationSpeed));
        rotationAnimation->set("to", newRotation);
        rotationAnimation->set("running", true);
    } else {
        mmain->logger()->log("Could not instantiate rotationAnimation in CcfQmlBase.");
    }
    mMoving = true;

    QObject *xMoveAnimation = child("xMoveAnimation");
    QObject *yMoveAnimation = child("yMoveAnimation");
    if (xMoveAnimation && yMoveAnimation) {
        qreal moveDuration = CcfEngineHelpers::targetDistance(x(), y(), tempX,
                                                              tempY) * 800 / (mMaxSpeed * factor);
        set("duration", moveDuration);
        set("duration", moveDuration);
    } else {
        mmain->logger()->log("Could not instantiate xMoveAnimation or yMoveAnimation in CcfQmlBase.");
    }
}

/*!
  Performs firing order by starting the turret rotation animation.

  When rotation stops, code in Tank.qml picks up and starts movement.
  */
void CcfQmlBaseUnit::performTurretShooting(qreal targetX, qreal targetY)
{
    QObject *tra = child("turretRotationAnimation");
    if (tra) {
        qreal newRotation = CcfEngineHelpers::rotationAngle(
                    x(), y(),
                    targetX - mCenterX,
                    targetY - mCenterY) - rotation();
        tra->set("duration", CcfEngineHelpers::rotationDuration(
                     getReal("turretRotation"),
                     newRotation,
                     getReal("turretRotationSpeed")));
        tra->set("to", newRotation);
        tra->set("running", true);
    } else {
        mmain->logger()->log("Could not instantiate turretRotationAnimation in CcfQmlBase.");
    }

    changeStatus("ROTATING");
}

/*!
  Calculates result of a hit
  */
void CcfQmlBaseUnit::hit(QObject *byWhat, qreal xWhere, qreal yWhere)
{
    Q_UNUSED(byWhat);
    Q_UNUSED(xWhere);
    Q_UNUSED(yWhere);

    // For now, not much logic is in ... :)
    cancelOrder();
    // Strangely, both state change calls invoke on base state only once!
    setState("destroyed");
    setState("destroyed_base");
}

/*!
  Returns a ready-made pointer to a new OrderMarker. Warning: needs to be reparented in
  QML in order to be visible!
 */
QObject *CcfQmlBaseUnit::createOrder(QObject *parent)
{
    if (mOrdersComponent->isReady()) {
        QObject *object = mOrdersComponent->create();
        object->set("parent", QVariant::fromValue(parent));
        return object;
    } else {
        return 0;
    }
}

/*!
  Cancels unit's orders, clears order queue.
  */
void CcfQmlBaseUnit::cancelOrder()
{
    changeStatus("STOPPED");
    clearOrderQueue();
    mMoving = false;

    if ((mFiring == false) && (mSmoking == false))  {
        QObject *xma = child("xMoveAnimation");
        if (xma) invoke(xma, "stop");
        QObject *yma = child("yMoveAnimation");
        if (yma) invoke(yma, "stop");
        QObject *ram = child("rotationAnimation");
        if (ram) invoke(ram, "stop");
        changeStatus("READY");
    }

    if ((mFiring == true) || (mSmoking == true))  {
        QObject *tra = child("turretRotationAnimation");
        if (tra) invoke(tra, "stop");
        mSmoking = false;
        mFiring = false;
        changeStatus("READY");
    }
}

/*!
  Puts a new order at the end of a queue.
  */
void CcfQmlBaseUnit::queueOrder(const QString &orderName, qreal x, qreal y, QObject *reparent)
{
    QObject *order = createOrder(reparent);
    if (order != 0) {
        order->set("index", mOrders.length());
        order->set("number", mOrders.length());
        order->set("operation", orderName);
        order->set("orderColor", CcfEngineHelpers::colorForOrder(orderName));

        order->set("x", (x - order->getReal("centerX")));
        order->set("y", (y - order->getReal("centerY")));
        order->set("visible", true);
        mOrders.append(order);
    }
}

/*!
  Processes next element in the queue.
  */
void CcfQmlBaseUnit::continueQueue()
{
    bool noOrdersLeft = true;

    for (int i = 0; i < mOrders.length(); ++i) {
        QObject *order = mOrders.value(i);
        if (order->getBool("performed") == true) {
            continue;
        } else {
            mCurrentOrder = i;

            qreal targetX = order->getReal("targetX");
            qreal targetY = order->getReal("targetY");
            QString operation = order->getString("operation");
            if (operation == "Move") {
                changeStatus("MOVING");
                performMovement(targetX, targetY, 1);
            } else if (operation == "Move fast") {
                changeStatus("MOVING FAST");
                performMovement(targetX, targetY, mMoveFastFactor);
            } else if (operation == "Sneak") {
                changeStatus("SNEAKING");
                performMovement(targetX, targetY, mSneakFactor);
            } else if (operation == "Smoke") {
                performTurretShooting(targetX, targetY);
                mSmoking = true;
            } else if (operation == "Attack") {
                performTurretShooting(targetX, targetY);
                mFiring = true;
            }

            order->set("performed", true);
            noOrdersLeft = false;
            // Ensures that unit performs one order at a time
            break;
        }
    }

    if (noOrdersLeft == true) {
        clearOrderQueue();
    }
}

/*!
  Makes sure that queue in execution is not disturbed
  by new calls. Called to begin queue execution.
  */
void CcfQmlBaseUnit::processQueue()
{
    if (mCurrentOrder == -1) {
        continueQueue();
    }
}

/*!
  Clears the order queue, deleting all orders inside.

  Sets currentOrder to -1.
  */
void CcfQmlBaseUnit::clearOrderQueue()
{
    for (int i = 0; i < mOrders.length(); ++i) {
        deleteOrder(i);
    }

    mCurrentOrder = -1;
    mOrders.clear();
}

/*!
  Deletes an order object found at \a index in the orders list.
  */
void CcfQmlBaseUnit::deleteOrder(int index)
{
    if (index < mOrders.length()) {
        delete mOrders.takeAt(index);  //.at(index);
    }
}

/*!
  Orders a unit to move. This puts the order in the queue.
  */
void CcfQmlBaseUnit::moveTo(qreal newX, qreal newY, QObject *reparent)
{
    emit movementBegan();
    queueOrder("Move", newX, newY, reparent);
    processQueue();
}

/*!
  Orders a unit to move fast. This puts the order in the queue.

  \sa moveFastFactor
  */
void CcfQmlBaseUnit::moveFastTo(qreal newX, qreal newY, QObject *reparent)
{
    emit movementBegan();
    queueOrder("Move fast", newX, newY, reparent);
    processQueue();
}

/*!
  Orders a unit to sneak. This puts the order in the queue.

  \sa sneakFactor
  */
void CcfQmlBaseUnit::sneakTo(qreal newX, qreal newY, QObject *reparent)
{
    emit movementBegan();
    queueOrder("Sneak", newX, newY, reparent);
    processQueue();
}

/*!
  Orders a unit to fire. This puts the order in the queue.
  */
void CcfQmlBaseUnit::fireTo(qreal targetX, qreal targetY, QObject *reparent)
{
    queueOrder("Attack", targetX, targetY, reparent);
    processQueue();
}

/*!
  Orders a unit to place smoke. This puts the order in the queue.
  */
void CcfQmlBaseUnit::smokeTo(qreal targetX, qreal targetY, QObject *reparent)
{
    queueOrder("Smoke", targetX, targetY, reparent);
    processQueue();
}

/*!
  Return a list of soldiers in this unit.
  */
QVariantList CcfQmlBaseUnit::soldiers()
{
    if (mSoldiers.isEmpty()) {
        // Get soldiers up front:
        QObjectList kids = children();

        for (int i = 0; i < kids.length(); ++i) {
            if (kids.at(i)->metaObject()->className() == QString("CcfQmlBaseSoldier"))
                mSoldiers.append(QVariant::fromValue(kids.at(i)));
        }
    }

    return mSoldiers;
}

/*!
  Toggles unit's pause state.
  */
void CcfQmlBaseUnit::togglePause()
{
    mPaused = !mPaused;
    emit pausedChanged();
}

// Property getters:
/*!
  Returns object type.
  */
QString CcfQmlBaseUnit::getObjectType() const
{
    return mObjectType;
}

/*!
  Returns unit's file name.
  */
QString CcfQmlBaseUnit::getUnitFileName() const
{
    return mUnitFileName;
}

/*!
  Returns unit's type.
  */
QString CcfQmlBaseUnit::getUnitType() const
{
    return mUnitType;
}

/*!
  Returns the path to unit's logo.
  */
QString CcfQmlBaseUnit::getUnitLogo() const
{
    return mUnitLogo;
}

/*!
  Returns unit's status.
  */
QString CcfQmlBaseUnit::getUnitStatus() const
{
    return mUnitStatus;
}

/*!
  Return's the side this unit is on.
  */
QString CcfQmlBaseUnit::getUnitSide() const
{
    return mUnitSide;
}

/*!
  Returns the number of the group this unit is part of. Defaults to 0.
  */
int CcfQmlBaseUnit::getGroupNumber() const
{
    return mGroupNumber;
}

/*!
  Return's unit's index, as set by the scenario.
  */
int CcfQmlBaseUnit::getUnitIndex() const
{
    return mUnitIndex;
}

/*!
  Returns true if the side mark is visible.
  */
bool CcfQmlBaseUnit::getSideMarkVisible() const
{
    return mSideMarkVisible;
}

/*!
  Returns the path to the side mark image.
  */
QString CcfQmlBaseUnit::getSideMarkSource() const
{
    return mSideMarkSource;
}

/*!
  Returns the side mark set in use.
  */
QString CcfQmlBaseUnit::getSideMarkSet() const
{
    return mSideMarkSet;
}

/*!
  Returns unit's rotation speed.
  */
int CcfQmlBaseUnit::getRotationSpeed() const
{
    return mRotationSpeed;
}

/*!
  Returns the turret's rotation speed.
  */
int CcfQmlBaseUnit::getTurretRotationSpeed() const
{
    return mTurretRotationSpeed;
}

/*!
  Returns unit's maximum speed.
  */
int CcfQmlBaseUnit::getMaxSpeed() const
{
    return mMaxSpeed;
}

/*!
  Returns unit's acceleration.
  */
int CcfQmlBaseUnit::getAcceleration() const
{
    return mAcceleration;
}

/*!
  Returns unit's width. This is not necessarily uniform with the QML component's width.
  */
int CcfQmlBaseUnit::getUnitWidth() const
{
    return mUnitWidth;
}

/*!
  Returns unit's height. This is not necessarily uniform with the QML component's height.
  */
int CcfQmlBaseUnit::getUnitHeight() const
{
    return mUnitHeight;
}

/*!
  Returns the move fast speed factor.
  */
qreal CcfQmlBaseUnit::getMoveFastFactor() const
{
    return mMoveFastFactor;
}

/*!
  Returns the sneak speed factor.
  */
qreal CcfQmlBaseUnit::getSneakFactor() const
{
    return mSneakFactor;
}

/*!
  Returns the cental x coordinate.
  */
int CcfQmlBaseUnit::getCenterX() const
{
    return mCenterX;
}

/*!
  Returns the central y coordinate.
  */
int CcfQmlBaseUnit::getCenterY() const
{
    return mCenterY;
}

/*!
  Returns current order number.
  */
int CcfQmlBaseUnit::getCurrentOrder() const
{
    return mCurrentOrder;
}

/*!
  Returns true if unit is selected.
  */
bool CcfQmlBaseUnit::getSelected() const
{
    return mSelected;
}

/*!
  Returns true if the unit is firing.
  */
bool CcfQmlBaseUnit::getFiring() const
{
    return mFiring;
}

/*!
  Returns true if the unit is smoking.
  */
bool CcfQmlBaseUnit::getSmoking() const
{
    return mSmoking;
}

/*!
  Returns defence sphere's rotation in degrees.
  */
int CcfQmlBaseUnit::getDefenceSphereRotation() const
{
    return mDefenceSphereRotation;
}

/*!
  Returns the defence sphere's colour.
  */
QString CcfQmlBaseUnit::getDefenceSphereColor() const
{
    return mDefenceSphereColor;
}

/*!
  Returns true if the unit is paused.
  */
bool CcfQmlBaseUnit::getPaused() const
{
    return mPaused;
}

/*!
  Returns true if the unit is moving.
  */
bool CcfQmlBaseUnit::getMoving() const
{
    return mMoving;
}

// Property setters:
/*!
  Sets object type.
  */
void CcfQmlBaseUnit::setObjectType(const QString &objectType)
{
    bool wasChaged = false;
    if (objectType != mObjectType)
        wasChaged = true;

    mObjectType = objectType;

    if (wasChaged)
        emit objectTypeChanged();
}

/*!
  Sets unit's file name.
  */
void CcfQmlBaseUnit::setUnitFileName(const QString &unitFileName)
{
    bool wasChaged = false;
    if (unitFileName != mUnitFileName)
        wasChaged = true;

    mUnitFileName = unitFileName;

    if (wasChaged)
        emit unitFileNameChanged();
}

/*!
  Sets unit type.
  */
void CcfQmlBaseUnit::setUnitType(const QString &unitType)
{
    bool wasChaged = false;
    if (unitType != mUnitType)
        wasChaged = true;

    mUnitType = unitType;

    if (wasChaged)
        emit unitTypeChanged();
}

/*!
  Sets the path to unit's logo.
  */
void CcfQmlBaseUnit::setUnitLogo(const QString &unitLogo)
{
    bool wasChaged = false;
    if (unitLogo != mUnitLogo)
        wasChaged = true;

    mUnitLogo = unitLogo;

    if (wasChaged)
        emit unitLogoChanged();
}

/*!
  Sets unit's status.
  */
void CcfQmlBaseUnit::setUnitStatus(const QString &unitStatus)
{
    bool wasChaged = false;
    if (unitStatus != mUnitStatus)
        wasChaged = true;

    mUnitStatus = unitStatus;

    if (wasChaged)
        emit unitStatusChanged();
}

/*!
  Sets the side this unit is on.
  */
void CcfQmlBaseUnit::setUnitSide(const QString &unitSide)
{
    bool wasChaged = false;
    if (unitSide != mUnitSide)
        wasChaged = true;

    mUnitSide = unitSide;

    if (wasChaged)
        emit unitSideChanged();
}

/*!
  Sets the group this unit belongs to.
  */
void CcfQmlBaseUnit::setGroupNumber(int groupNumber)
{
    bool wasChaged = false;
    if (groupNumber != mGroupNumber)
        wasChaged = true;

    mGroupNumber = groupNumber;

    if (wasChaged)
        emit groupNumberChanged();
}

/*!
  Sets unit's index.
  */
void CcfQmlBaseUnit::setUnitIndex(int unitIndex)
{
    bool wasChaged = false;
    if (unitIndex != mUnitIndex)
        wasChaged = true;

    mUnitIndex = unitIndex;

    if (wasChaged)
        emit unitIndexChanged();
}

/*!
  Sets the visibility of unit's side mark.
  */
void CcfQmlBaseUnit::setSideMarkVisible(bool sideMarkVisible)
{
    bool wasChaged = false;
    if (sideMarkVisible != mSideMarkVisible)
        wasChaged = true;

    mSideMarkVisible = sideMarkVisible;

    if (wasChaged)
        emit sideMarkVisibleChanged();
}

/*!
  Sets the source path to unit's side mark.
  */
void CcfQmlBaseUnit::setSideMarkSource(const QString &sideMarkSource)
{
    bool wasChaged = false;
    if (sideMarkSource != mSideMarkSource)
        wasChaged = true;

    mSideMarkSource = sideMarkSource;

    if (wasChaged)
        emit sideMarkSourceChanged();
}

/*!
  Sets the set in use for side mark.
  */
void CcfQmlBaseUnit::setSideMarkSet(const QString &sideMarkSet)
{
    bool wasChaged = false;
    if (sideMarkSet != mSideMarkSet)
        wasChaged = true;

    mSideMarkSet = sideMarkSet;

    if (wasChaged)
        emit sideMarkSetChanged();
}

/*!
  Sets unit's rotation speed.
  */
void CcfQmlBaseUnit::setRotationSpeed(int rotationSpeed)
{
    bool wasChaged = false;
    if (rotationSpeed != mRotationSpeed)
        wasChaged = true;

    mRotationSpeed = rotationSpeed;

    if (wasChaged)
        emit rotationSpeedChanged();
}

/*!
  Sets unit turret's rotation speed.
  */
void CcfQmlBaseUnit::setTurretRotationSpeed(int turretRotationSpeed)
{
    bool wasChaged = false;
    if (turretRotationSpeed != mTurretRotationSpeed)
        wasChaged = true;

    mTurretRotationSpeed = turretRotationSpeed;

    if (wasChaged)
        emit turretRotationSpeedChanged();
}

/*!
  Sets unit's maximum speed.
  */
void CcfQmlBaseUnit::setMaxSpeed(int maxSpeed)
{
    bool wasChaged = false;
    if (maxSpeed != mMaxSpeed)
        wasChaged = true;

    mMaxSpeed = maxSpeed;

    if (wasChaged)
        emit maxSpeedChanged();
}

/*!
  Sets acceleration.
  */
void CcfQmlBaseUnit::setAcceleration(int acceleration)
{
    bool wasChaged = false;
    if (acceleration != mAcceleration)
        wasChaged = true;

    mAcceleration = acceleration;

    if (wasChaged)
        emit accelerationChanged();
}

/*!
  Sets unit's width.
  */
void CcfQmlBaseUnit::setUnitWidth(int unitWidth)
{
    bool wasChaged = false;
    if (unitWidth != mUnitWidth)
        wasChaged = true;

    mUnitWidth = unitWidth;

    if (wasChaged)
        emit unitWidthChanged();
}

/*!
  Sets unit's heigth.
  */
void CcfQmlBaseUnit::setUnitHeight(int unitHeight)
{
    bool wasChaged = false;
    if (unitHeight != mUnitHeight)
        wasChaged = true;

    mUnitHeight = unitHeight;

    if (wasChaged)
        emit unitHeightChanged();
}

/*!
  Sets the factor for move fast speed.
  */
void CcfQmlBaseUnit::setMoveFastFactor(qreal moveFastFactor)
{
    bool wasChaged = false;
    if (moveFastFactor != mMoveFastFactor)
        wasChaged = true;

    mMoveFastFactor = moveFastFactor;

    if (wasChaged)
        emit moveFastFactorChanged();
}

/*!
  Sets the factor for sneking speed.
  */
void CcfQmlBaseUnit::setSneakFactor(qreal sneakFactor)
{
    bool wasChaged = false;
    if (sneakFactor != mSneakFactor)
        wasChaged = true;

    mSneakFactor = sneakFactor;

    if (wasChaged)
        emit sneakFactorChanged();
}

/*!
  Sets x coord of unit's central point.
  */
void CcfQmlBaseUnit::setCenterX(int centerX)
{
    bool wasChaged = false;
    if (centerX != mCenterX)
        wasChaged = true;

    mCenterX = centerX;

    if (wasChaged)
        emit centerXChanged();
}

/*!
  Sets y coord for unit's central point.
  */
void CcfQmlBaseUnit::setCenterY(int centerY)
{
    bool wasChaged = false;
    if (centerY != mCenterY)
        wasChaged = true;

    mCenterY = centerY;

    if (wasChaged)
        emit centerYChanged();
}

/*!
  Sets current order index.
  */
void CcfQmlBaseUnit::setCurrentOrder(int currentOrder)
{
    bool wasChaged = false;
    if (currentOrder != mCurrentOrder)
        wasChaged = true;

    mCurrentOrder = currentOrder;

    if (wasChaged)
        emit currentOrderChanged();
}

/*!
  Makes the unit selected or not.
  */
void CcfQmlBaseUnit::setSelected(bool selected)
{
    bool wasChaged = false;
    if (selected != mSelected)
        wasChaged = true;

    mSelected = selected;

    if (wasChaged)
        emit selectedChanged(mSelected, mUnitIndex);
}

/*!
  Sets unit's firing status.
  */
void CcfQmlBaseUnit::setFiring(bool firing)
{
    bool wasChaged = false;
    if (firing != mFiring)
        wasChaged = true;

    mFiring = firing;

    if (wasChaged)
        emit firingChanged();
}

/*!
  Sets unit's smoking status.
  */
void CcfQmlBaseUnit::setSmoking(bool smoking)
{
    bool wasChaged = false;
    if (smoking != mSmoking)
        wasChaged = true;

    mSmoking = smoking;

    if (wasChaged)
        emit smokingChanged();
}

/*!
  Sets defence sphere's rotation in degrees.
  */
void CcfQmlBaseUnit::setDefenceSphereRotation(int defenceSphereRotation)
{
    bool wasChaged = false;
    if (defenceSphereRotation != mDefenceSphereRotation)
        wasChaged = true;

    mDefenceSphereRotation = defenceSphereRotation;

    if (wasChaged)
        emit defenceSphereRotationChanged();
}

/*!
  Sets defence sphere's colour.
  */
void CcfQmlBaseUnit::setDefenceSphereColor(const QString &defenceSphereColor)
{
    bool wasChaged = false;
    if (defenceSphereColor != mDefenceSphereColor)
        wasChaged = true;

    mDefenceSphereColor = defenceSphereColor;

    if (wasChaged)
        emit defenceSphereColorChanged();
}

/*!
  Sets the unit to be paused or not.
  */
void CcfQmlBaseUnit::setPaused(bool paused)
{
    bool wasChaged = false;
    if (paused != mPaused)
        wasChaged = true;

    mPaused = paused;

    if (wasChaged)
        emit pausedChanged();
}

/*!
  Sets unit's moving state..
  */
void CcfQmlBaseUnit::setMoving(bool moving)
{
    bool wasChaged = false;
    if (moving != mMoving)
        wasChaged = true;

    mMoving = moving;

    if (wasChaged) {
        emit movingChanged();
        emit movementStateChange(mMoving, mUnitIndex);
    }
}

/*!
  Checks whether the given \a index is in range of the order's list, and returns
  true if it is indeed valid.
  */
bool CcfQmlBaseUnit::isOrderIndexValid(int index) const
{
    if ((index >= 0) && (index < mOrders.length()))
        return true;
    return false;
}
