#ifndef CCFQMLBASESOLDIER_H
#define CCFQMLBASESOLDIER_H

#include <QString>
#include <QObject>

/*!
  \defgroup CloseCombatFree Game code
  @{
 */

/*!
  This is a base object for Soldire.qml file.
  */
class CcfQmlBaseSoldier : public QObject
{
    Q_OBJECT

public:
    /*!
      Returns the type of the object, as all other CCF objects do.

      // TODO: Consider making an abstract base class to hold basic properties.
      */
    Q_PROPERTY(QString objectType READ getObjectType WRITE setObjectType NOTIFY objectTypeChanged)

    /*!
      Soldier's name.
      */
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)

    /*!
      Soldier's role (gunner, loader, etc.).
      */
    Q_PROPERTY(QString role READ getRole WRITE setRole NOTIFY roleChanged)

    /*!
      Soldier's status.
      */
    Q_PROPERTY(QString status READ getStatus WRITE setStatus NOTIFY statusChanged)

    /*!
      Soldier's weapon.
      */
    Q_PROPERTY(QString weapon READ getWeapon WRITE setWeapon NOTIFY weaponChanged)

    /*!
      Path to a file with soldier's logo.

      // TODO: change into QIcon, maybe?
      */
    Q_PROPERTY(QString soldierLogo READ getSoldierLogo WRITE setSoldierLogo NOTIFY soldierLogoChanged)

    explicit CcfQmlBaseSoldier(QObject *parent = 0);

    Q_INVOKABLE void changeStatus(const QString &newStatus);

    QString getObjectType() const;
    QString getName() const;
    QString getRole() const;
    QString getStatus() const;
    QString getWeapon() const;
    QString getSoldierLogo() const;

    void setObjectType(const QString &objectType);
    void setName(const QString &name);
    void setRole(const QString &role);
    void setStatus(const QString &status);
    void setWeapon(const QString &weapon);
    void setSoldierLogo(const QString &soldierLogo);

signals:
    /*!
      Emitted when unit's status changes.
      */
    void unitStatusChanged(const QString &newStatus);

    /*!
      Emitted when object's type changes.
      */
    void objectTypeChanged();

    /*!
      Emitted when name is changed.
      */
    void nameChanged();

    /*!
      Emitted when role is changed.
      */
    void roleChanged();

    /*!
      Emitted when the status is changed.
      */
    void statusChanged();

    /*!
      Emitted when the weapon is changed.
      */
    void weaponChanged();

    /*!
      Emitted when the path to soldier's logo changes.
      */
    void soldierLogoChanged();

private:
    QString mObjectType;
    QString mName;
    QString mRole;
    QString mStatus;
    QString mWeapon;
    QString mSoldierLogo;
};

/*! @}*/

#endif // CCFQMLBASESOLDIER_H
