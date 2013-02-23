#ifndef CCFCOMMANDLINEPARSER_H
#define CCFCOMMANDLINEPARSER_H

#include <QStringList>

/*!
  \defgroup CloseCombatFree Game code
  @{
 */

/*!
  Class responsible for parsing the command line.
  */
class CcfCommandLineParser
{
public:
    CcfCommandLineParser(const QStringList &args);

    bool isDebug();
    bool wasHelpRequested();
    QString helpMessage();

private:
    void init(const QStringList &args);

    QStringList mSwitches;
};

/*! @}*/

#endif // CCFCOMMANDLINEPARSER_H
