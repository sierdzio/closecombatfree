#ifndef CCFQMLBASESOLDIER_H
#define CCFQMLBASESOLDIER_H

#include <QtCore/QString>
#include <QtCore/QObject>

/*!
  \defgroup CloseCombatFree Game code
  @{
 */

class CcfQmlBaseSoldier : public QObject
{
    Q_OBJECT

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

public:
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
    void unitStatusChanged(const QString &newStatus);
    void objectTypeChanged();
    void nameChanged();
    void roleChanged();
    void statusChanged();
    void weaponChanged();
    void soldierLogoChanged();

private:
    QString m_objectType;
    QString m_name;
    QString m_role;
    QString m_status;
    QString m_weapon;
    QString m_soldierLogo;
};

/*! @}*/

#endif // CCFQMLBASESOLDIER_H
