#include "ccfqmlbasesoldier.h"

CcfQmlBaseSoldier::CcfQmlBaseSoldier(QObject *parent) : QObject(parent)
{    
    m_objectType = "soldierInfo";
    m_name = "John Smith";
    m_role = "Infantryman";
    m_status = "READY";
    m_weapon = "";
    m_soldierLogo = "../../../img/units/generic_soldier_logo.png";
}

/*!
  Changes current status of the soldier.
  */
void CcfQmlBaseSoldier::changeStatus(const QString &newStatus)
{
    m_status = newStatus;
    emit unitStatusChanged(newStatus);
}

QString CcfQmlBaseSoldier::getObjectType() const
{
    return m_objectType;
}

QString CcfQmlBaseSoldier::getName() const
{
    return m_name;
}

QString CcfQmlBaseSoldier::getRole() const
{
    return m_role;
}

QString CcfQmlBaseSoldier::getStatus() const
{
    return m_status;
}

QString CcfQmlBaseSoldier::getWeapon() const
{
    return m_weapon;
}

QString CcfQmlBaseSoldier::getSoldierLogo() const
{
    return m_soldierLogo;
}

void CcfQmlBaseSoldier::setObjectType(const QString &objectType)
{
    bool wasChaged = false;
    if (objectType != m_objectType)
        wasChaged = true;

    m_objectType = objectType;

    if (wasChaged)
        emit objectTypeChanged();
}

void CcfQmlBaseSoldier::setName(const QString &name)
{
    bool wasChaged = false;
    if (name != m_name)
        wasChaged = true;

    m_name = name;

    if (wasChaged)
        emit nameChanged();
}

void CcfQmlBaseSoldier::setRole(const QString &role)
{
    bool wasChaged = false;
    if (role != m_role)
        wasChaged = true;

    m_role = role;

    if (wasChaged)
        emit roleChanged();
}

void CcfQmlBaseSoldier::setStatus(const QString &status)
{
    bool wasChaged = false;
    if (status != m_status)
        wasChaged = true;

    m_status = status;

    if (wasChaged)
        emit statusChanged();
}

void CcfQmlBaseSoldier::setWeapon(const QString &weapon)
{
    bool wasChaged = false;
    if (weapon != m_weapon)
        wasChaged = true;

    m_weapon = weapon;

    if (wasChaged)
        emit weaponChanged();
}

void CcfQmlBaseSoldier::setSoldierLogo(const QString &soldierLogo)
{
    bool wasChaged = false;
    if (soldierLogo != m_soldierLogo)
        wasChaged = true;

    m_soldierLogo = soldierLogo;

    if (wasChaged)
        emit soldierLogoChanged();
}
