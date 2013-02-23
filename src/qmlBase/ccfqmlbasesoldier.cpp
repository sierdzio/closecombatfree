#include "ccfqmlbasesoldier.h"

/*!
  Constructs a new BaseSoldier. Includes some default property values.
  */
CcfQmlBaseSoldier::CcfQmlBaseSoldier(QObject *parent) : QObject(parent)
{    
    mObjectType = "soldierInfo";
    mName = "John Smith";
    mRole = "Infantryman";
    mStatus = "READY";
    mWeapon = "";
    mSoldierLogo = "../../../img/units/generic_soldier_logo.png";
}

/*!
  Changes current status of the soldier.
  */
void CcfQmlBaseSoldier::changeStatus(const QString &newStatus)
{
    mStatus = newStatus;
    emit unitStatusChanged(newStatus);
}

/*!
  Returns the object's type.
  */
QString CcfQmlBaseSoldier::getObjectType() const
{
    return mObjectType;
}

/*!
  Returns the name.
  */
QString CcfQmlBaseSoldier::getName() const
{
    return mName;
}

/*!
  Returns the role.
  */
QString CcfQmlBaseSoldier::getRole() const
{
    return mRole;
}

/*!
  Returns the status.
  */
QString CcfQmlBaseSoldier::getStatus() const
{
    return mStatus;
}

/*!
  Returns the weapon.
  */
QString CcfQmlBaseSoldier::getWeapon() const
{
    return mWeapon;
}

/*!
  Returns the soldier logo's path.
  */
QString CcfQmlBaseSoldier::getSoldierLogo() const
{
    return mSoldierLogo;
}

/*!
  Sets the \a objectType.
  */
void CcfQmlBaseSoldier::setObjectType(const QString &objectType)
{
    bool wasChaged = false;
    if (objectType != mObjectType)
        wasChaged = true;

    mObjectType = objectType;

    if (wasChaged)
        emit objectTypeChanged();
}

/*!
  Sets the \a name.
  */
void CcfQmlBaseSoldier::setName(const QString &name)
{
    bool wasChaged = false;
    if (name != mName)
        wasChaged = true;

    mName = name;

    if (wasChaged)
        emit nameChanged();
}

/*!
  Sets the \a role.
  */
void CcfQmlBaseSoldier::setRole(const QString &role)
{
    bool wasChaged = false;
    if (role != mRole)
        wasChaged = true;

    mRole = role;

    if (wasChaged)
        emit roleChanged();
}

/*!
  Sets the \a status.
  */
void CcfQmlBaseSoldier::setStatus(const QString &status)
{
    bool wasChaged = false;
    if (status != mStatus)
        wasChaged = true;

    mStatus = status;

    if (wasChaged)
        emit statusChanged();
}

/*!
  Sets the \a weapon.
  */
void CcfQmlBaseSoldier::setWeapon(const QString &weapon)
{
    bool wasChaged = false;
    if (weapon != mWeapon)
        wasChaged = true;

    mWeapon = weapon;

    if (wasChaged)
        emit weaponChanged();
}

/*!
  Sets the \a soldierLogo (a path to where the logo picture is located).
  */
void CcfQmlBaseSoldier::setSoldierLogo(const QString &soldierLogo)
{
    bool wasChaged = false;
    if (soldierLogo != mSoldierLogo)
        wasChaged = true;

    mSoldierLogo = soldierLogo;

    if (wasChaged)
        emit soldierLogoChanged();
}
