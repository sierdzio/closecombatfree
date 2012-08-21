#ifndef CCFLOGGER_H
#define CCFLOGGER_H

#include <QtCore/QObject>

class CcfLogger : public QObject
{
    Q_OBJECT
public:
    explicit CcfLogger(QObject *parent = 0, bool isDebugOn = false);

    Q_INVOKABLE void log(const QString &message);
//    Q_INVOKABLE void debug(const QString &message);
    
private:
    bool isDebug;
};

#endif // CCFLOGGER_H
