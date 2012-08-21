#include <QtCore/QDir>

#include "ccfscenariostate.h"

/*!
  QObject constructor + sets vars to their default state.
 */
CcfScenarioState::CcfScenarioState(QObject *parent) :
    QObject(parent)
{
    m_playerSide = "neutral";
    m_availableSides.append("neutral");

    // Get side mark files that are available
    QDir sideMarkDir("./img/units/sideMarks", "sideMark_");
    m_availableSideMarkFiles = sideMarkDir.entryList();
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

/*!
  Returns a list of player sides available in this scenario.

  \sa setAvailableScenarios
 */
QStringList CcfScenarioState::getAvailableSides()
{
    return m_availableSides;
}

/*!
  Sets a list of player sides using \a availableSides, cleans any duplicated
  entries found inside, and stores it as current state.
 */
void CcfScenarioState::setAvailableSides(QStringList availableSides)
{
    availableSides.removeDuplicates();
    m_availableSides = availableSides;
}

QString CcfScenarioState::getSidePath(const QString &side)
{
    QString pathBeginning("../../img/units/sideMarks/sideMark_");
    int sideIndex = m_availableSides.indexOf(side);

    if (sideIndex == -1)
        return (pathBeginning + "side1.png");

    // TODO: add support for sideMark set choosing (additional property in unit,
    // that would name the set to use. Then filename is: <sideSet><index>.png).
    QString result(pathBeginning + "side" + QString::number(sideIndex + 1) + ".png");
    return result;
}
