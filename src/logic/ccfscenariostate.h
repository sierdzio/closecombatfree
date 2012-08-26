#ifndef CCFSCENARIOSTATE_H
#define CCFSCENARIOSTATE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

/*!
  \defgroup CloseCombatFree Game code
  @{
 */

/*!
  The CcfScenarioState class stores run-time info about current scenario.
 */
class CcfScenarioState : public QObject
{
    Q_OBJECT

    /*!
      Stores the side our player is controlling.
      */
    Q_PROPERTY(QString playerSide READ getPlayerSide WRITE setPlayerSide NOTIFY playerSideChanged)

    /*!
      Stores the side our player is controlling.
      */
    Q_PROPERTY(QStringList availableSides READ getAvailableSides WRITE setAvailableSides NOTIFY availableSidesChanged)

public:
    explicit CcfScenarioState(QObject *parent = 0);

protected:
    Q_INVOKABLE QString getPlayerSide();
    Q_INVOKABLE void setPlayerSide(const QString &playerSide);
    Q_INVOKABLE QStringList getAvailableSides();
    Q_INVOKABLE void setAvailableSides(QStringList availableSides);
//    Q_INVOKABLE void setAvailableSides(const QList<QObject *> &items);
    Q_INVOKABLE QString getSidePath(const QString &side, const QString &sideMarkSet = "side");

signals:
    Q_INVOKABLE void playerSideChanged();
    Q_INVOKABLE void availableSidesChanged();

private:
    QString m_playerSide;
    QStringList m_availableSides;
};

/*! @}*/

#endif // CCFSCENARIOSTATE_H
