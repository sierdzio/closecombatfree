#include <QtCore/QFile>

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

/*!
  Returns an image path for a given \a side and optionally \a sideMarkSet.

  Checks for path's validity, too, and tries to fix it if wrong data is given.
 */
QString CcfScenarioState::getSidePath(const QString &side, const QString &sideMarkSet)
{
    QString pathBeginning("../../img/units/sideMarks/sideMark_");
    int sideIndex = m_availableSides.indexOf(side);

    if (sideIndex == -1)
        return (pathBeginning + "side_1.png");

    QString result(sideMarkSet + "_" + QString::number(sideIndex + 1) + ".png");

    QString execPathBeginning("img/units/sideMarks/sideMark_");
    if (!QFile::exists(execPathBeginning + result)) {
        result = QStringLiteral("side_") + QString::number(sideIndex + 1) + ".png";

        if (!QFile::exists(execPathBeginning + result)) {
            result = "side_1.png";
        }
    }

    return (pathBeginning + result);
}
