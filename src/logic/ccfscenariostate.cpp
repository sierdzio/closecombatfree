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

QStringList CcfScenarioState::getAvailableSides()
{
    return m_availableSides;
}

void CcfScenarioState::setAvailableSides(QStringList availableSides)
{
//    int dups =
    availableSides.removeDuplicates();
//    qDebug(qPrintable("Dups count: %s"), qPrintable(QString::number(dups)));
//    foreach (const QString &s, availableSides) {
//        qDebug(qPrintable("Side name: %s"), qPrintable(s));
//    }

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
