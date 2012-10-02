#include "ccflogger.h"

CcfLogger::CcfLogger(QObject *parent, bool isDebugOn) :
    QObject(parent), isDebug(isDebugOn)
{
}

/*!
  Logs a given \a message, optionally prepended by \a additional data.

  Traditionally, additional data is used to provide log description based on
  a movie name.
  */
void CcfLogger::log(const QString &message, const QString &additionalData)
{
    QString result;
    if (additionalData.isEmpty())
        result = message;
    else
        result = additionalData + " - " + message;

    // TODO: add logging! :)

    // TODO: clean c++ and QML, and JS files of console.logs. Decide what is debug, and what
    // is log. Period. :)

    if (isDebug)
        qDebug(qPrintable(result), NULL);
}
