#ifndef CCFLOGGER_H
#define CCFLOGGER_H

#include <QtCore/QObject>

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

    bool isDebug;
};

/*! @}*/

#endif // CCFLOGGER_H
