#ifndef CCFSCENARIOSTATE_H
#define CCFSCENARIOSTATE_H

#include <QObject>
#include <QString>
#include <QStringList>

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

    Q_INVOKABLE QString getPlayerSide();
    Q_INVOKABLE void setPlayerSide(const QString &playerSide);
    Q_INVOKABLE QStringList getAvailableSides();
    Q_INVOKABLE void setAvailableSides(QStringList availableSides);
    Q_INVOKABLE QString getSidePath(const QString &side, const QString &sideMarkSet = "side");

signals:
    /*!
      Emitted when player changes sides.
      */
    void playerSideChanged();

    /*!
      Emitted when the list of available sides changes.
      */
    void availableSidesChanged();

private:
    QString mPlayerSide;
    QStringList mAvailableSides;
};

/*! @}*/

#endif // CCFSCENARIOSTATE_H
