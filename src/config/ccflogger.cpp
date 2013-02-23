#include "ccflogger.h"

/*!
  Initialises the logger with QObject \a parent and an optional parameter \a isDebugOn,
  which (if set to true) instructs the class to print all messages into the console,
  apart from doing other standard logging actions.
  */
CcfLogger::CcfLogger(QObject *parent, bool isDebugOn) :
    QObject(parent), mIsDebug(isDebugOn)
{
}

/*!
  Logs a given \a message, optionally prepended by \a additional data.

  Traditionally, additional data is used to provide log description based on
  a movie name.

  \sa error
  */
void CcfLogger::log(const QString &message, const QString &additionalData)
{
    addLogEntry(Log, message, additionalData);
}

/*!
  Logs a given \a message, optionally prepended by \a additional data, and prints
  the same log line into console output.

  Traditionally, additional data is used to provide log description based on
  a movie name.

  \sa log
  */
void CcfLogger::error(const QString &message, const QString &additionalData)
{
    addLogEntry(Error, message, additionalData);
}

void CcfLogger::addLogEntry(CcfLogger::MessageType msgType, const QString &message, const QString &additionalData)
{
    QString result;
    if (additionalData.isEmpty())
        result = message;
    else
        result = additionalData + " - " + message;

    // TODO: add logging! :)

    // TODO: clean c++ and QML, and JS files of console.logs. Decide what is debug, and what
    // is log. Period. :)

    if (mIsDebug || (msgType == Error))
        qDebug(qPrintable(result), NULL);
}
