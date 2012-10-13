#include "ccfqmlbaseunit.h"
#include "ccfmain.h"
#include "logic/ccfenginehelpers.h"

#include <QtQml/QQmlComponent>

/*!
  BaseUnit constructor - initialises all properties with default values,
  prepares orderMarker component for object generation.
  */
CcfQmlBaseUnit::CcfQmlBaseUnit(QQuickItem *parent) : CcfObjectBase(parent)
{
    m_objectType = QStringLiteral("unit");
    m_unitFileName = QStringLiteral("Unit");
    m_unitType = QStringLiteral("Generic unit");
    m_unitLogo = QStringLiteral("../../img/units/generic_unit_logo.png");
    m_unitStatus = QStringLiteral("READY");
    m_unitSide = QStringLiteral("neutral");
    m_groupNumber = 0;
    m_unitIndex = -1;
    m_sideMarkVisible = true;
    m_sideMarkSource = QStringLiteral("../../img/units/sideMarks/sideMark_side_1.png");
    m_sideMarkSet = QStringLiteral("side");
    m_rotationSpeed = 1;
    m_turretRotationSpeed = 1;
    m_maxSpeed = 1;
    m_acceleration = 1;
    m_unitWidth = 1;
    m_unitHeight = 1;
    m_moveFastFactor = 1.5;
    m_sneakFactor = 0.4;
    m_currentOrder = -1;
    m_selected = false;
    m_firing = false;
    m_smoking = false;
    m_defenceSphereRotation = 0;
    m_paused = false;
    m_moving = false;

    m_mainInstance = CcfMain::instance();
    m_ordersComponent = new QQmlComponent(m_mainInstance->engine(),
                                          QUrl::fromLocalFile("qml/gui/OrderMarker.qml"));
}

/*!
  Returns the name of the operation associated with order \a index, or current order
  if no index is given.
  */
QString CcfQmlBaseUnit::operation(int index) const
{
    if (index == -1) {
        if (m_currentOrder != -1)
            index = m_currentOrder;
        else
            index = 0;
    }

    if (isOrderIndexValid(index)) {
        return m_orders.at(index)->getString("operation");
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
        index = m_currentOrder;

    int x = 0;
    int y = 0;
    if (isOrderIndexValid(index)) {
        x = m_orders.at(index)->getInt("targetX");
        y = m_orders.at(index)->getInt("targetY");
    }

    return QPoint(x, y);
}

/*!
  Changes unit's status.
  */
void CcfQmlBaseUnit::changeStatus(const QString &newStatusMessage)
{
    m_unitStatus = newStatusMessage;
    emit unitStatusChanged(newStatusMessage, m_unitIndex);
}

/*!
  Performs movement order by starting the rotation animation.

  When rotation stops, code in Unit.qml picks up and starts movement.
  */
void CcfQmlBaseUnit::performMovement(qreal newX, qreal newY, qreal factor)
{
    qreal tempX = newX - (m_centerX);
    qreal tempY = newY - (m_centerY);

    qreal newRotation = CcfEngineHelpers::rotationAngle(x(), y(), tempX, tempY);
    QObject *rotationAnimation = child("rotationAnimation");
    if (rotationAnimation) {
        rotationAnimation->set("duration",
                               CcfEngineHelpers::rotationDuration(rotation(),
                                                                  newRotation, m_rotationSpeed));
        rotationAnimation->set("to", newRotation);
        rotationAnimation->set("running", true);
    } else {
        mmain->logger()->log("Could not instantiate rotationAnimation in CcfQmlBase.");
    }
    m_moving = true;

    QObject *xMoveAnimation = child("xMoveAnimation");
    QObject *yMoveAnimation = child("yMoveAnimation");
    if (xMoveAnimation && yMoveAnimation) {
        qreal moveDuration = CcfEngineHelpers::targetDistance(x(), y(), tempX,
                                                              tempY) * 800 / (m_maxSpeed * factor);
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
                    targetX - m_centerX,
                    targetY - m_centerY) - rotation();
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
    if (m_ordersComponent->isReady()) {
        QObject *object = m_ordersComponent->create();
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
    m_moving = false;

    if ((m_firing == false) && (m_smoking == false))  {
        QObject *xma = child("xMoveAnimation");
        if (xma) invoke(xma, "stop");
        QObject *yma = child("yMoveAnimation");
        if (yma) invoke(yma, "stop");
        QObject *ram = child("rotationAnimation");
        if (ram) invoke(ram, "stop");
        changeStatus("READY");
    }

    if ((m_firing == true) || (m_smoking == true))  {
        QObject *tra = child("turretRotationAnimation");
        if (tra) invoke(tra, "stop");
        m_smoking = false;
        m_firing = false;
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
        order->set("index", m_orders.length());
        order->set("number", m_orders.length());
        order->set("operation", orderName);
        order->set("orderColor", CcfEngineHelpers::colorForOrder(orderName));

        order->set("x", (x - order->getReal("centerX")));
        order->set("y", (y - order->getReal("centerY")));
        order->set("visible", true);
        m_orders.append(order);
    }
}

/*!
  Processes next element in the queue.
  */
void CcfQmlBaseUnit::continueQueue()
{
    bool noOrdersLeft = true;

    for (int i = 0; i < m_orders.length(); ++i) {
        QObject *order = m_orders.value(i);
        if (order->getBool("performed") == true) {
            continue;
        } else {
            m_currentOrder = i;

            qreal targetX = order->getReal("targetX");
            qreal targetY = order->getReal("targetY");
            QString operation = order->getString("operation");
            if (operation == "Move") {
                changeStatus("MOVING");
                performMovement(targetX, targetY, 1);
            } else if (operation == "Move fast") {
                changeStatus("MOVING FAST");
                performMovement(targetX, targetY, m_moveFastFactor);
            } else if (operation == "Sneak") {
                changeStatus("SNEAKING");
                performMovement(targetX, targetY, m_sneakFactor);
            } else if (operation == "Smoke") {
                performTurretShooting(targetX, targetY);
                m_smoking = true;
            } else if (operation == "Attack") {
                performTurretShooting(targetX, targetY);
                m_firing = true;
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
    if (m_currentOrder == -1) {
        continueQueue();
    }
}

/*!
  Clears the order queue, deleting all orders inside.

  Sets currentOrder to -1.
  */
void CcfQmlBaseUnit::clearOrderQueue()
{
    for (int i = 0; i < m_orders.length(); ++i) {
        deleteOrder(i);
    }

    m_currentOrder = -1;
    m_orders.clear();
}

/*!
  Deletes an order object found at \a index in the orders list.
  */
void CcfQmlBaseUnit::deleteOrder(int index)
{
    if (index < m_orders.length()) {
        delete m_orders.takeAt(index);  //.at(index);
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
    if (m_soldiers.isEmpty()) {
        // Get soldiers up front:
        QObjectList kids = children();

        for (int i = 0; i < kids.length(); ++i) {
            if (kids.at(i)->metaObject()->className() == QString("CcfQmlBaseSoldier"))
                m_soldiers.append(QVariant::fromValue(kids.at(i)));
        }
    }

    return m_soldiers;
}

/*!
  Toggles unit's pause state.
  */
void CcfQmlBaseUnit::togglePause()
{
    m_paused = !m_paused;
    emit pausedChanged();
}

// Property getters:
/*!
  Returns object type.
  */
QString CcfQmlBaseUnit::getObjectType() const
{
    return m_objectType;
}

/*!
  Returns unit's file name.
  */
QString CcfQmlBaseUnit::getUnitFileName() const
{
    return m_unitFileName;
}

/*!
  Returns unit's type.
  */
QString CcfQmlBaseUnit::getUnitType() const
{
    return m_unitType;
}

/*!
  Returns the path to unit's logo.
  */
QString CcfQmlBaseUnit::getUnitLogo() const
{
    return m_unitLogo;
}

/*!
  Returns unit's status.
  */
QString CcfQmlBaseUnit::getUnitStatus() const
{
    return m_unitStatus;
}

/*!
  Return's the side this unit is on.
  */
QString CcfQmlBaseUnit::getUnitSide() const
{
    return m_unitSide;
}

/*!
  Returns the number of the group this unit is part of. Defaults to 0.
  */
int CcfQmlBaseUnit::getGroupNumber() const
{
    return m_groupNumber;
}

/*!
  Return's unit's index, as set by the scenario.
  */
int CcfQmlBaseUnit::getUnitIndex() const
{
    return m_unitIndex;
}

/*!
  Returns true if the side mark is visible.
  */
bool CcfQmlBaseUnit::getSideMarkVisible() const
{
    return m_sideMarkVisible;
}

/*!
  Returns the path to the side mark image.
  */
QString CcfQmlBaseUnit::getSideMarkSource() const
{
    return m_sideMarkSource;
}

/*!
  Returns the side mark set in use.
  */
QString CcfQmlBaseUnit::getSideMarkSet() const
{
    return m_sideMarkSet;
}

/*!
  Returns unit's rotation speed.
  */
int CcfQmlBaseUnit::getRotationSpeed() const
{
    return m_rotationSpeed;
}

/*!
  Returns the turret's rotation speed.
  */
int CcfQmlBaseUnit::getTurretRotationSpeed() const
{
    return m_turretRotationSpeed;
}

/*!
  Returns unit's maximum speed.
  */
int CcfQmlBaseUnit::getMaxSpeed() const
{
    return m_maxSpeed;
}

/*!
  Returns unit's acceleration.
  */
int CcfQmlBaseUnit::getAcceleration() const
{
    return m_acceleration;
}

/*!
  Returns unit's width. This is not necessarily uniform with the QML component's width.
  */
int CcfQmlBaseUnit::getUnitWidth() const
{
    return m_unitWidth;
}

/*!
  Returns unit's height. This is not necessarily uniform with the QML component's height.
  */
int CcfQmlBaseUnit::getUnitHeight() const
{
    return m_unitHeight;
}

/*!
  Returns the move fast speed factor.
  */
qreal CcfQmlBaseUnit::getMoveFastFactor() const
{
    return m_moveFastFactor;
}

/*!
  Returns the sneak speed factor.
  */
qreal CcfQmlBaseUnit::getSneakFactor() const
{
    return m_sneakFactor;
}

/*!
  Returns the cental x coordinate.
  */
int CcfQmlBaseUnit::getCenterX() const
{
    return m_centerX;
}

/*!
  Returns the central y coordinate.
  */
int CcfQmlBaseUnit::getCenterY() const
{
    return m_centerY;
}

/*!
  Returns current order number.
  */
int CcfQmlBaseUnit::getCurrentOrder() const
{
    return m_currentOrder;
}

/*!
  Returns true if unit is selected.
  */
bool CcfQmlBaseUnit::getSelected() const
{
    return m_selected;
}

/*!
  Returns true if the unit is firing.
  */
bool CcfQmlBaseUnit::getFiring() const
{
    return m_firing;
}

/*!
  Returns true if the unit is smoking.
  */
bool CcfQmlBaseUnit::getSmoking() const
{
    return m_smoking;
}

/*!
  Returns defence sphere's rotation in degrees.
  */
int CcfQmlBaseUnit::getDefenceSphereRotation() const
{
    return m_defenceSphereRotation;
}

/*!
  Returns the defence sphere's colour.
  */
QString CcfQmlBaseUnit::getDefenceSphereColor() const
{
    return m_defenceSphereColor;
}

/*!
  Returns true if the unit is paused.
  */
bool CcfQmlBaseUnit::getPaused() const
{
    return m_paused;
}

/*!
  Returns true if the unit is moving.
  */
bool CcfQmlBaseUnit::getMoving() const
{
    return m_moving;
}

// Property setters:
/*!
  Sets object type.
  */
void CcfQmlBaseUnit::setObjectType(const QString &objectType)
{
    bool wasChaged = false;
    if (objectType != m_objectType)
        wasChaged = true;

    m_objectType = objectType;

    if (wasChaged)
        emit objectTypeChanged();
}

/*!
  Sets unit's file name.
  */
void CcfQmlBaseUnit::setUnitFileName(const QString &unitFileName)
{
    bool wasChaged = false;
    if (unitFileName != m_unitFileName)
        wasChaged = true;

    m_unitFileName = unitFileName;

    if (wasChaged)
        emit unitFileNameChanged();
}

/*!
  Sets unit type.
  */
void CcfQmlBaseUnit::setUnitType(const QString &unitType)
{
    bool wasChaged = false;
    if (unitType != m_unitType)
        wasChaged = true;

    m_unitType = unitType;

    if (wasChaged)
        emit unitTypeChanged();
}

/*!
  Sets the path to unit's logo.
  */
void CcfQmlBaseUnit::setUnitLogo(const QString &unitLogo)
{
    bool wasChaged = false;
    if (unitLogo != m_unitLogo)
        wasChaged = true;

    m_unitLogo = unitLogo;

    if (wasChaged)
        emit unitLogoChanged();
}

/*!
  Sets unit's status.
  */
void CcfQmlBaseUnit::setUnitStatus(const QString &unitStatus)
{
    bool wasChaged = false;
    if (unitStatus != m_unitStatus)
        wasChaged = true;

    m_unitStatus = unitStatus;

    if (wasChaged)
        emit unitStatusChanged();
}

/*!
  Sets the side this unit is on.
  */
void CcfQmlBaseUnit::setUnitSide(const QString &unitSide)
{
    bool wasChaged = false;
    if (unitSide != m_unitSide)
        wasChaged = true;

    m_unitSide = unitSide;

    if (wasChaged)
        emit unitSideChanged();
}

/*!
  Sets the group this unit belongs to.
  */
void CcfQmlBaseUnit::setGroupNumber(int groupNumber)
{
    bool wasChaged = false;
    if (groupNumber != m_groupNumber)
        wasChaged = true;

    m_groupNumber = groupNumber;

    if (wasChaged)
        emit groupNumberChanged();
}

/*!
  Sets unit's index.
  */
void CcfQmlBaseUnit::setUnitIndex(int unitIndex)
{
    bool wasChaged = false;
    if (unitIndex != m_unitIndex)
        wasChaged = true;

    m_unitIndex = unitIndex;

    if (wasChaged)
        emit unitIndexChanged();
}

/*!
  Sets the visibility of unit's side mark.
  */
void CcfQmlBaseUnit::setSideMarkVisible(bool sideMarkVisible)
{
    bool wasChaged = false;
    if (sideMarkVisible != m_sideMarkVisible)
        wasChaged = true;

    m_sideMarkVisible = sideMarkVisible;

    if (wasChaged)
        emit sideMarkVisibleChanged();
}

/*!
  Sets the source path to unit's side mark.
  */
void CcfQmlBaseUnit::setSideMarkSource(const QString &sideMarkSource)
{
    bool wasChaged = false;
    if (sideMarkSource != m_sideMarkSource)
        wasChaged = true;

    m_sideMarkSource = sideMarkSource;

    if (wasChaged)
        emit sideMarkSourceChanged();
}

/*!
  Sets the set in use for side mark.
  */
void CcfQmlBaseUnit::setSideMarkSet(const QString &sideMarkSet)
{
    bool wasChaged = false;
    if (sideMarkSet != m_sideMarkSet)
        wasChaged = true;

    m_sideMarkSet = sideMarkSet;

    if (wasChaged)
        emit sideMarkSetChanged();
}

/*!
  Sets unit's rotation speed.
  */
void CcfQmlBaseUnit::setRotationSpeed(int rotationSpeed)
{
    bool wasChaged = false;
    if (rotationSpeed != m_rotationSpeed)
        wasChaged = true;

    m_rotationSpeed = rotationSpeed;

    if (wasChaged)
        emit rotationSpeedChanged();
}

/*!
  Sets unit turret's rotation speed.
  */
void CcfQmlBaseUnit::setTurretRotationSpeed(int turretRotationSpeed)
{
    bool wasChaged = false;
    if (turretRotationSpeed != m_turretRotationSpeed)
        wasChaged = true;

    m_turretRotationSpeed = turretRotationSpeed;

    if (wasChaged)
        emit turretRotationSpeedChanged();
}

/*!
  Sets unit's maximum speed.
  */
void CcfQmlBaseUnit::setMaxSpeed(int maxSpeed)
{
    bool wasChaged = false;
    if (maxSpeed != m_maxSpeed)
        wasChaged = true;

    m_maxSpeed = maxSpeed;

    if (wasChaged)
        emit maxSpeedChanged();
}

/*!
  Sets acceleration.
  */
void CcfQmlBaseUnit::setAcceleration(int acceleration)
{
    bool wasChaged = false;
    if (acceleration != m_acceleration)
        wasChaged = true;

    m_acceleration = acceleration;

    if (wasChaged)
        emit accelerationChanged();
}

/*!
  Sets unit's width.
  */
void CcfQmlBaseUnit::setUnitWidth(int unitWidth)
{
    bool wasChaged = false;
    if (unitWidth != m_unitWidth)
        wasChaged = true;

    m_unitWidth = unitWidth;

    if (wasChaged)
        emit unitWidthChanged();
}

/*!
  Sets unit's heigth.
  */
void CcfQmlBaseUnit::setUnitHeight(int unitHeight)
{
    bool wasChaged = false;
    if (unitHeight != m_unitHeight)
        wasChaged = true;

    m_unitHeight = unitHeight;

    if (wasChaged)
        emit unitHeightChanged();
}

/*!
  Sets the factor for move fast speed.
  */
void CcfQmlBaseUnit::setMoveFastFactor(qreal moveFastFactor)
{
    bool wasChaged = false;
    if (moveFastFactor != m_moveFastFactor)
        wasChaged = true;

    m_moveFastFactor = moveFastFactor;

    if (wasChaged)
        emit moveFastFactorChanged();
}

/*!
  Sets the factor for sneking speed.
  */
void CcfQmlBaseUnit::setSneakFactor(qreal sneakFactor)
{
    bool wasChaged = false;
    if (sneakFactor != m_sneakFactor)
        wasChaged = true;

    m_sneakFactor = sneakFactor;

    if (wasChaged)
        emit sneakFactorChanged();
}

/*!
  Sets x coord of unit's central point.
  */
void CcfQmlBaseUnit::setCenterX(int centerX)
{
    bool wasChaged = false;
    if (centerX != m_centerX)
        wasChaged = true;

    m_centerX = centerX;

    if (wasChaged)
        emit centerXChanged();
}

/*!
  Sets y coord for unit's central point.
  */
void CcfQmlBaseUnit::setCenterY(int centerY)
{
    bool wasChaged = false;
    if (centerY != m_centerY)
        wasChaged = true;

    m_centerY = centerY;

    if (wasChaged)
        emit centerYChanged();
}

/*!
  Sets current order index.
  */
void CcfQmlBaseUnit::setCurrentOrder(int currentOrder)
{
    bool wasChaged = false;
    if (currentOrder != m_currentOrder)
        wasChaged = true;

    m_currentOrder = currentOrder;

    if (wasChaged)
        emit currentOrderChanged();
}

/*!
  Makes the unit selected or not.
  */
void CcfQmlBaseUnit::setSelected(bool selected)
{
    bool wasChaged = false;
    if (selected != m_selected)
        wasChaged = true;

    m_selected = selected;

    if (wasChaged)
        emit selectedChanged(m_selected, m_unitIndex);
}

/*!
  Sets unit's firing status.
  */
void CcfQmlBaseUnit::setFiring(bool firing)
{
    bool wasChaged = false;
    if (firing != m_firing)
        wasChaged = true;

    m_firing = firing;

    if (wasChaged)
        emit firingChanged();
}

/*!
  Sets unit's smoking status.
  */
void CcfQmlBaseUnit::setSmoking(bool smoking)
{
    bool wasChaged = false;
    if (smoking != m_smoking)
        wasChaged = true;

    m_smoking = smoking;

    if (wasChaged)
        emit smokingChanged();
}

/*!
  Sets defence sphere's rotation in degrees.
  */
void CcfQmlBaseUnit::setDefenceSphereRotation(int defenceSphereRotation)
{
    bool wasChaged = false;
    if (defenceSphereRotation != m_defenceSphereRotation)
        wasChaged = true;

    m_defenceSphereRotation = defenceSphereRotation;

    if (wasChaged)
        emit defenceSphereRotationChanged();
}

/*!
  Sets defence sphere's colour.
  */
void CcfQmlBaseUnit::setDefenceSphereColor(const QString &defenceSphereColor)
{
    bool wasChaged = false;
    if (defenceSphereColor != m_defenceSphereColor)
        wasChaged = true;

    m_defenceSphereColor = defenceSphereColor;

    if (wasChaged)
        emit defenceSphereColorChanged();
}

/*!
  Sets the unit to be paused or not.
  */
void CcfQmlBaseUnit::setPaused(bool paused)
{
    bool wasChaged = false;
    if (paused != m_paused)
        wasChaged = true;

    m_paused = paused;

    if (wasChaged)
        emit pausedChanged();
}

/*!
  Sets unit's moving state..
  */
void CcfQmlBaseUnit::setMoving(bool moving)
{
    bool wasChaged = false;
    if (moving != m_moving)
        wasChaged = true;

    m_moving = moving;

    if (wasChaged) {
        emit movingChanged();
        emit movementStateChange(m_moving, m_unitIndex);
    }
}

/*!
  Checks whether the given \a index is in range of the order's list, and returns
  true if it is indeed valid.
  */
bool CcfQmlBaseUnit::isOrderIndexValid(int index) const
{
    if ((index >= 0) && (index < m_orders.length()))
        return true;
    return false;
}
