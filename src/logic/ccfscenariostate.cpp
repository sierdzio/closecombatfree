#include "ccfscenariostate.h"

CcfScenarioState::CcfScenarioState(QObject *parent) :
    QObject(parent)
{
    m_playerSide = "neutral";
}

QString CcfScenarioState::getPlayerSide()
{
    return m_playerSide;
}

void CcfScenarioState::setPlayerSide(const QString &playerSide)
{
    bool wasChaged = false;
    if (playerSide != m_playerSide)
        wasChaged = true;

    m_playerSide = playerSide;

    if (wasChaged)
        emit playerSideChanged();
}
