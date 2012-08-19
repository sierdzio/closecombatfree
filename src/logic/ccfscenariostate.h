#ifndef CCFSCENARIOSTATE_H
#define CCFSCENARIOSTATE_H

#include <QtCore/QObject>
#include <QtCore/QString>

class CcfScenarioState : public QObject
{
    Q_OBJECT

    /*!
      Stores the side our player is controlling.
      */
    Q_PROPERTY(QString playerSide READ getPlayerSide WRITE setPlayerSide NOTIFY playerSideChanged)

public:
    explicit CcfScenarioState(QObject *parent = 0);

protected:
    Q_INVOKABLE QString getPlayerSide();
    Q_INVOKABLE void setPlayerSide(const QString &playerSide);

signals:
    Q_INVOKABLE void playerSideChanged();

private:
    QString m_playerSide;
};

#endif // CCFSCENARIOSTATE_H
