#include "ccfqmlbasesoldier.h"

/*!
  Constructs a new BaseSoldier. Includes some default property values.
  */
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

/*!
  Returns the object's type.
  */
QString CcfQmlBaseSoldier::getObjectType() const
{
    return m_objectType;
}

/*!
  Returns the name.
  */
QString CcfQmlBaseSoldier::getName() const
{
    return m_name;
}

/*!
  Returns the role.
  */
QString CcfQmlBaseSoldier::getRole() const
{
    return m_role;
}

/*!
  Returns the status.
  */
QString CcfQmlBaseSoldier::getStatus() const
{
    return m_status;
}

/*!
  Returns the weapon.
  */
QString CcfQmlBaseSoldier::getWeapon() const
{
    return m_weapon;
}

/*!
  Returns the soldier logo's path.
  */
QString CcfQmlBaseSoldier::getSoldierLogo() const
{
    return m_soldierLogo;
}

/*!
  Sets the \a objectType.
  */
void CcfQmlBaseSoldier::setObjectType(const QString &objectType)
{
    bool wasChaged = false;
    if (objectType != m_objectType)
        wasChaged = true;

    m_objectType = objectType;

    if (wasChaged)
        emit objectTypeChanged();
}

/*!
  Sets the \a name.
  */
void CcfQmlBaseSoldier::setName(const QString &name)
{
    bool wasChaged = false;
    if (name != m_name)
        wasChaged = true;

    m_name = name;

    if (wasChaged)
        emit nameChanged();
}

/*!
  Sets the \a role.
  */
void CcfQmlBaseSoldier::setRole(const QString &role)
{
    bool wasChaged = false;
    if (role != m_role)
        wasChaged = true;

    m_role = role;

    if (wasChaged)
        emit roleChanged();
}

/*!
  Sets the \a status.
  */
void CcfQmlBaseSoldier::setStatus(const QString &status)
{
    bool wasChaged = false;
    if (status != m_status)
        wasChaged = true;

    m_status = status;

    if (wasChaged)
        emit statusChanged();
}

/*!
  Sets the \a weapon.
  */
void CcfQmlBaseSoldier::setWeapon(const QString &weapon)
{
    bool wasChaged = false;
    if (weapon != m_weapon)
        wasChaged = true;

    m_weapon = weapon;

    if (wasChaged)
        emit weaponChanged();
}

/*!
  Sets the \a soldierLogo (a path to where the logo picture is located).
  */
void CcfQmlBaseSoldier::setSoldierLogo(const QString &soldierLogo)
{
    bool wasChaged = false;
    if (soldierLogo != m_soldierLogo)
        wasChaged = true;

    m_soldierLogo = soldierLogo;

    if (wasChaged)
        emit soldierLogoChanged();
}
