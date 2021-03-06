#include "ccfcommandlineparser.h"

/*!
  Initialises the cmd line parser with a list of arguments (\a args).
  */
CcfCommandLineParser::CcfCommandLineParser(const QStringList &args)
{
    init(args);
}

/*!
  Returns true if debug flag is set.
  */
bool CcfCommandLineParser::isDebug()
{
    return mSwitches.contains("debug");
}

/*!
  Returns true if help flag is set.
  */
bool CcfCommandLineParser::wasHelpRequested()
{
    return mSwitches.contains("help");
}

/*!
  Returns a string (UNIX-terminated lines) containing the help message to be
  displayed.
  */
QString CcfCommandLineParser::helpMessage()
{
    // TODO: check on Windows, cause it can have problems with lack of \r\n...
    return QString("CloseCombatFree - command line help.\n"
                   "\n"
                   "Usage: closecombatfree [-h] [-d]\n"
                   "Explanation:\n"
                   " - h (--help)  - display this message and exit\n"
                   " - d (--debug) - print debug messages here (including log messages)\n"
                   );
}

/*!
  Parses the command line.
  */
void CcfCommandLineParser::init(const QStringList &args)
{
    mSwitches.clear();

    // Foreach instead of ::contains, to get a single-pass.
    foreach (const QString &s, args) {
        QString lower = s.toLower();

        if (lower == "-d" || lower == "--debug") {
            mSwitches.append("debug");
        } else if (lower == "-h" || lower == "--help") {
            mSwitches.append("help");
        }
    }
}
