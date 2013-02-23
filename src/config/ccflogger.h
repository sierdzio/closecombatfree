#ifndef CCFLOGGER_H
#define CCFLOGGER_H

#include <QObject>

/*!
  \defgroup CloseCombatFree Game code
  @{
 */

/*!
  Intended for providing logging and debug capabilities - across C++, QML and JS
  code of CCF. So far it's rather basic.
  */
class CcfLogger : public QObject
{
    Q_OBJECT
public:
    /*!
      An enum defining possible message types. This is used by addLogEntry() to
      determine, how the message should be processed (for example, Log messages
      are logged, but not pushed to the cerr output, while Error messages are logged
      and print out in the console).
      */
    enum MessageType {
        Log   = 0,
        Error = 1
    };

    explicit CcfLogger(QObject *parent = 0, bool isDebugOn = false);

    Q_INVOKABLE void log(const QString &message, const QString &additionalData = QString());
    Q_INVOKABLE void error(const QString &message, const QString &additionalData = QString());
    
private:
    void addLogEntry(MessageType msgType, const QString &message,
                     const QString &additionalData = QString());

    bool mIsDebug;
};

/*! @}*/

#endif // CCFLOGGER_H
