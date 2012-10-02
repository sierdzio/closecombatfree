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
    if ((index == -1) || (!isOrderIndexValid(index)))
        index = m_currentOrder;
    return m_orders.at(index)->getString("operation");
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

void CcfQmlBaseUnit::togglePause()
{
    m_paused = !m_paused;
    emit pausedChanged();
}

// Property getters:
QString CcfQmlBaseUnit::getObjectType() const
{
    return m_objectType;
}

QString CcfQmlBaseUnit::getUnitFileName() const
{
    return m_unitFileName;
}

QString CcfQmlBaseUnit::getUnitType() const
{
    return m_unitType;
}

QString CcfQmlBaseUnit::getUnitLogo() const
{
    return m_unitLogo;
}

QString CcfQmlBaseUnit::getUnitStatus() const
{
    return m_unitStatus;
}

QString CcfQmlBaseUnit::getUnitSide() const
{
    return m_unitSide;
}

int CcfQmlBaseUnit::getGroupNumber() const
{
    return m_groupNumber;
}

int CcfQmlBaseUnit::getUnitIndex() const
{
    return m_unitIndex;
}

bool CcfQmlBaseUnit::getSideMarkVisible() const
{
    return m_sideMarkVisible;
}

QString CcfQmlBaseUnit::getSideMarkSource() const
{
    return m_sideMarkSource;
}

QString CcfQmlBaseUnit::getSideMarkSet() const
{
    return m_sideMarkSet;
}

int CcfQmlBaseUnit::getRotationSpeed() const
{
    return m_rotationSpeed;
}

int CcfQmlBaseUnit::getTurretRotationSpeed() const
{
    return m_turretRotationSpeed;
}

int CcfQmlBaseUnit::getMaxSpeed() const
{
    return m_maxSpeed;
}

int CcfQmlBaseUnit::getAcceleration() const
{
    return m_acceleration;
}

int CcfQmlBaseUnit::getUnitWidth() const
{
    return m_unitWidth;
}

int CcfQmlBaseUnit::getUnitHeight() const
{
    return m_unitHeight;
}

qreal CcfQmlBaseUnit::getMoveFastFactor() const
{
    return m_moveFastFactor;
}

qreal CcfQmlBaseUnit::getSneakFactor() const
{
    return m_sneakFactor;
}

int CcfQmlBaseUnit::getCenterX() const
{
    return m_centerX;
}

int CcfQmlBaseUnit::getCenterY() const
{
    return m_centerY;
}

QString CcfQmlBaseUnit::getScheduledOperation() const
{
    return m_scheduledOperation;
}

int CcfQmlBaseUnit::getCurrentOrder() const
{
    return m_currentOrder;
}

bool CcfQmlBaseUnit::getSelected() const
{
    return m_selected;
}

bool CcfQmlBaseUnit::getFiring() const
{
    return m_firing;
}

bool CcfQmlBaseUnit::getSmoking() const
{
    return m_smoking;
}

int CcfQmlBaseUnit::getDefenceSphereRotation() const
{
    return m_defenceSphereRotation;
}

QString CcfQmlBaseUnit::getDefenceSphereColor() const
{
    return m_defenceSphereColor;
}

bool CcfQmlBaseUnit::getPaused() const
{
    return m_paused;
}

bool CcfQmlBaseUnit::getMoving() const
{
    return m_moving;
}

// Property setters:

void CcfQmlBaseUnit::setObjectType(const QString &objectType)
{
    bool wasChaged = false;
    if (objectType != m_objectType)
        wasChaged = true;

    m_objectType = objectType;

    if (wasChaged)
        emit objectTypeChanged();
}

void CcfQmlBaseUnit::setUnitFileName(const QString &unitFileName)
{
    bool wasChaged = false;
    if (unitFileName != m_unitFileName)
        wasChaged = true;

    m_unitFileName = unitFileName;

    if (wasChaged)
        emit unitFileNameChanged();
}

void CcfQmlBaseUnit::setUnitType(const QString &unitType)
{
    bool wasChaged = false;
    if (unitType != m_unitType)
        wasChaged = true;

    m_unitType = unitType;

    if (wasChaged)
        emit unitTypeChanged();
}

void CcfQmlBaseUnit::setUnitLogo(const QString &unitLogo)
{
    bool wasChaged = false;
    if (unitLogo != m_unitLogo)
        wasChaged = true;

    m_unitLogo = unitLogo;

    if (wasChaged)
        emit unitLogoChanged();
}

void CcfQmlBaseUnit::setUnitStatus(const QString &unitStatus)
{
    bool wasChaged = false;
    if (unitStatus != m_unitStatus)
        wasChaged = true;

    m_unitStatus = unitStatus;

    if (wasChaged)
        emit unitStatusChanged();
}

void CcfQmlBaseUnit::setUnitSide(const QString &unitSide)
{
    bool wasChaged = false;
    if (unitSide != m_unitSide)
        wasChaged = true;

    m_unitSide = unitSide;

    if (wasChaged)
        emit unitSideChanged();
}

void CcfQmlBaseUnit::setGroupNumber(int groupNumber)
{
    bool wasChaged = false;
    if (groupNumber != m_groupNumber)
        wasChaged = true;

    m_groupNumber = groupNumber;

    if (wasChaged)
        emit groupNumberChanged();
}

void CcfQmlBaseUnit::setUnitIndex(int unitIndex)
{
    bool wasChaged = false;
    if (unitIndex != m_unitIndex)
        wasChaged = true;

    m_unitIndex = unitIndex;

    if (wasChaged)
        emit unitIndexChanged();
}

void CcfQmlBaseUnit::setSideMarkVisible(bool sideMarkVisible)
{
    bool wasChaged = false;
    if (sideMarkVisible != m_sideMarkVisible)
        wasChaged = true;

    m_sideMarkVisible = sideMarkVisible;

    if (wasChaged)
        emit sideMarkVisibleChanged();
}

void CcfQmlBaseUnit::setSideMarkSource(const QString &sideMarkSource)
{
    bool wasChaged = false;
    if (sideMarkSource != m_sideMarkSource)
        wasChaged = true;

    m_sideMarkSource = sideMarkSource;

    if (wasChaged)
        emit sideMarkSourceChanged();
}

void CcfQmlBaseUnit::setSideMarkSet(const QString &sideMarkSet)
{
    bool wasChaged = false;
    if (sideMarkSet != m_sideMarkSet)
        wasChaged = true;

    m_sideMarkSet = sideMarkSet;

    if (wasChaged)
        emit sideMarkSetChanged();
}

void CcfQmlBaseUnit::setRotationSpeed(int rotationSpeed)
{
    bool wasChaged = false;
    if (rotationSpeed != m_rotationSpeed)
        wasChaged = true;

    m_rotationSpeed = rotationSpeed;

    if (wasChaged)
        emit rotationSpeedChanged();
}

void CcfQmlBaseUnit::setTurretRotationSpeed(int turretRotationSpeed)
{
    bool wasChaged = false;
    if (turretRotationSpeed != m_turretRotationSpeed)
        wasChaged = true;

    m_turretRotationSpeed = turretRotationSpeed;

    if (wasChaged)
        emit turretRotationSpeedChanged();
}

void CcfQmlBaseUnit::setMaxSpeed(int maxSpeed)
{
    bool wasChaged = false;
    if (maxSpeed != m_maxSpeed)
        wasChaged = true;

    m_maxSpeed = maxSpeed;

    if (wasChaged)
        emit maxSpeedChanged();
}

void CcfQmlBaseUnit::setAcceleration(int acceleration)
{
    bool wasChaged = false;
    if (acceleration != m_acceleration)
        wasChaged = true;

    m_acceleration = acceleration;

    if (wasChaged)
        emit accelerationChanged();
}

void CcfQmlBaseUnit::setUnitWidth(int unitWidth)
{
    bool wasChaged = false;
    if (unitWidth != m_unitWidth)
        wasChaged = true;

    m_unitWidth = unitWidth;

    if (wasChaged)
        emit unitWidthChanged();
}

void CcfQmlBaseUnit::setUnitHeight(int unitHeight)
{
    bool wasChaged = false;
    if (unitHeight != m_unitHeight)
        wasChaged = true;

    m_unitHeight = unitHeight;

    if (wasChaged)
        emit unitHeightChanged();
}

void CcfQmlBaseUnit::setMoveFastFactor(qreal moveFastFactor)
{
    bool wasChaged = false;
    if (moveFastFactor != m_moveFastFactor)
        wasChaged = true;

    m_moveFastFactor = moveFastFactor;

    if (wasChaged)
        emit moveFastFactorChanged();
}

void CcfQmlBaseUnit::setSneakFactor(qreal sneakFactor)
{
    bool wasChaged = false;
    if (sneakFactor != m_sneakFactor)
        wasChaged = true;

    m_sneakFactor = sneakFactor;

    if (wasChaged)
        emit sneakFactorChanged();
}

void CcfQmlBaseUnit::setCenterX(int centerX)
{
    bool wasChaged = false;
    if (centerX != m_centerX)
        wasChaged = true;

    m_centerX = centerX;

    if (wasChaged)
        emit centerXChanged();
}

void CcfQmlBaseUnit::setCenterY(int centerY)
{
    bool wasChaged = false;
    if (centerY != m_centerY)
        wasChaged = true;

    m_centerY = centerY;

    if (wasChaged)
        emit centerYChanged();
}

void CcfQmlBaseUnit::setScheduledOperation(const QString &scheduledOperation)
{
    bool wasChaged = false;
    if (scheduledOperation != m_scheduledOperation)
        wasChaged = true;

    m_scheduledOperation = scheduledOperation;

    if (wasChaged)
        emit scheduledOperationChanged();
}

void CcfQmlBaseUnit::setCurrentOrder(int currentOrder)
{
    bool wasChaged = false;
    if (currentOrder != m_currentOrder)
        wasChaged = true;

    m_currentOrder = currentOrder;

    if (wasChaged)
        emit currentOrderChanged();
}

void CcfQmlBaseUnit::setSelected(bool selected)
{
    bool wasChaged = false;
    if (selected != m_selected)
        wasChaged = true;

    m_selected = selected;

    if (wasChaged)
        emit selectedChanged(m_selected, m_unitIndex);
}

void CcfQmlBaseUnit::setFiring(bool firing)
{
    bool wasChaged = false;
    if (firing != m_firing)
        wasChaged = true;

    m_firing = firing;

    if (wasChaged)
        emit firingChanged();
}

void CcfQmlBaseUnit::setSmoking(bool smoking)
{
    bool wasChaged = false;
    if (smoking != m_smoking)
        wasChaged = true;

    m_smoking = smoking;

    if (wasChaged)
        emit smokingChanged();
}

void CcfQmlBaseUnit::setDefenceSphereRotation(int defenceSphereRotation)
{
    bool wasChaged = false;
    if (defenceSphereRotation != m_defenceSphereRotation)
        wasChaged = true;

    m_defenceSphereRotation = defenceSphereRotation;

    if (wasChaged)
        emit defenceSphereRotationChanged();
}

void CcfQmlBaseUnit::setDefenceSphereColor(const QString &defenceSphereColor)
{
    bool wasChaged = false;
    if (defenceSphereColor != m_defenceSphereColor)
        wasChaged = true;

    m_defenceSphereColor = defenceSphereColor;

    if (wasChaged)
        emit defenceSphereColorChanged();
}

void CcfQmlBaseUnit::setPaused(bool paused)
{
    bool wasChaged = false;
    if (paused != m_paused)
        wasChaged = true;

    m_paused = paused;

    if (wasChaged)
        emit pausedChanged();
}

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

bool CcfQmlBaseUnit::isOrderIndexValid(int index) const
{
    if ((index >= 0) && (index < m_orders.length()))
        return true;
    return false;
}
