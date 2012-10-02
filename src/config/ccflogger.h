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
    explicit CcfLogger(QObject *parent = 0, bool isDebugOn = false);

    Q_INVOKABLE void log(const QString &message, const QString &additionalData = QString());
    
private:
    bool isDebug;
};

/*! @}*/

#endif // CCFLOGGER_H
