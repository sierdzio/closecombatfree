#include "ccflogger.h"

CcfLogger::CcfLogger(QObject *parent, bool isDebugOn) :
    QObject(parent), isDebug(isDebugOn)
{
}

void CcfLogger::log(const QString &message)
{
    // TODO: add logging! :)

    // TODO: clean c++ and QML, and JS files of console.logs. Decide what is debug, and what
    // is log. Period. :)

    if (isDebug)
        qDebug(qPrintable(message), NULL);
}
