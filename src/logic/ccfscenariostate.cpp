#include "ccfscenariostate.h"

/*!
  QObject constructor + sets vars to their default state.
 */
CcfScenarioState::CcfScenarioState(QObject *parent) :
    QObject(parent)
{
    m_playerSide = "neutral";
    m_availableSides.append("neutral");
}

/*!
  Returns the side player is currently on.
 */
QString CcfScenarioState::getPlayerSide()
{
    return m_playerSide;
}

/*!
  Sets current player side using \a playerSide.
 */
void CcfScenarioState::setPlayerSide(const QString &playerSide)
{
    bool wasChaged = false;
    if (playerSide != m_playerSide)
        wasChaged = true;

    m_playerSide = playerSide;

    if (wasChaged)
        emit playerSideChanged();
}

QStringList CcfScenarioState::getAvailableSides()
{
    return m_availableSides;
}

void CcfScenarioState::setAvailableSides(const QStringList &availableSides)
{
    m_availableSides = availableSides;
}
