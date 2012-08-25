#include "ccfqmlbaseunit.h"

CcfQmlBaseUnit::CcfQmlBaseUnit(QQuickItem *parent) :
    QQuickItem(parent)
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
}

// Property getters:

QString CcfQmlBaseUnit::getObjectType()
{
    return m_objectType;
}

QString CcfQmlBaseUnit::getUnitFileName()
{
    return m_unitFileName;
}

QString CcfQmlBaseUnit::getUnitType()
{
    return m_unitType;
}

QString CcfQmlBaseUnit::getUnitLogo()
{
    return m_unitLogo;
}

QString CcfQmlBaseUnit::getUnitStatus()
{
    return m_unitStatus;
}

QString CcfQmlBaseUnit::getUnitSide()
{
    return m_unitSide;
}

int CcfQmlBaseUnit::getGroupNumber()
{
    return m_groupNumber;
}

int CcfQmlBaseUnit::getUnitIndex()
{
    return m_unitIndex;
}

bool CcfQmlBaseUnit::getSideMarkVisible()
{
    return m_sideMarkVisible;
}

QString CcfQmlBaseUnit::getSideMarkSource()
{
    return m_sideMarkSource;
}

QString CcfQmlBaseUnit::getSideMarkSet()
{
    return m_sideMarkSet;
}

int CcfQmlBaseUnit::getRotationSpeed()
{
    return m_rotationSpeed;
}

int CcfQmlBaseUnit::getTurretRotationSpeed()
{
    return m_turretRotationSpeed;
}

int CcfQmlBaseUnit::getMaxSpeed()
{
    return m_maxSpeed;
}

int CcfQmlBaseUnit::getAcceleration()
{
    return m_acceleration;
}

int CcfQmlBaseUnit::getUnitWidth()
{
    return m_unitWidth;
}

int CcfQmlBaseUnit::getUnitHeight()
{
    return m_unitHeight;
}

//QQmlListReference CcfQmlBaseUnit::getSoldiers()
//{
//    return m_soldiers;
//}

//QVariantList CcfQmlBaseUnit::getOrders()
//{
////    qDebug("C++ debug! Length: %d", m_orders.count());
//    return m_orders;
//}

//int CcfQmlBaseUnit::ordersLength()
//{
//    return m_orders.count();
//}

//void CcfQmlBaseUnit::ordersClear()
//{
//    m_orders.clear();
//}

qreal CcfQmlBaseUnit::getMoveFastFactor()
{
    return m_moveFastFactor;
}

qreal CcfQmlBaseUnit::getSneakFactor()
{
    return m_sneakFactor;
}

int CcfQmlBaseUnit::getCenterX()
{
    return m_centerX;
}

int CcfQmlBaseUnit::getCenterY()
{
    return m_centerY;
}

int CcfQmlBaseUnit::getTempX()
{
    return m_tempX;
}

int CcfQmlBaseUnit::getTempY()
{
    return m_tempY;
}

QString CcfQmlBaseUnit::getScheduledOperation()
{
    return m_scheduledOperation;
}

int CcfQmlBaseUnit::getCurrentOrder()
{
    return m_currentOrder;
}

bool CcfQmlBaseUnit::getSelected()
{
    return m_selected;
}

bool CcfQmlBaseUnit::getFiring()
{
    return m_firing;
}

bool CcfQmlBaseUnit::getSmoking()
{
    return m_smoking;
}

int CcfQmlBaseUnit::getDefenceSphereRotation()
{
    return m_defenceSphereRotation;
}

QString CcfQmlBaseUnit::getDefenceSphereColor()
{
    return m_defenceSphereColor;
}

bool CcfQmlBaseUnit::getPaused()
{
    return m_paused;
}

bool CcfQmlBaseUnit::getMoving()
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

//void CcfQmlBaseUnit::setSoldiers(const QQmlListReference &soldiers)
//{
//    m_soldiers = soldiers;
//    emit soldiersChanged();
//}

//void CcfQmlBaseUnit::setOrders(QVariantList orders)
//{
//    m_orders = orders;
//    emit ordersChanged();
//}

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

void CcfQmlBaseUnit::setTempX(int tempX)
{
    bool wasChaged = false;
    if (tempX != m_tempX)
        wasChaged = true;

    m_tempX = tempX;

    if (wasChaged)
        emit tempXChanged();
}

void CcfQmlBaseUnit::setTempY(int tempY)
{
    bool wasChaged = false;
    if (tempY != m_tempY)
        wasChaged = true;

    m_tempY = tempY;

    if (wasChaged)
        emit tempYChanged();
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
        emit selectedChanged();
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

    if (wasChaged)
        emit movingChanged();
}
