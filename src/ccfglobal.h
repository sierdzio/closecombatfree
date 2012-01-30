#ifndef CCFGLOBAL_H
#define CCFGLOBAL_H

#include <QObject>
#include <QString>

#include "ccferror.h"

class CcfGlobal : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit CcfGlobal(QObject *parent = 0);
    Q_INVOKABLE void disableQrcUse(QObject *object);

public slots:
    void statusMsg(const QString &message);
    void statusMessage(const QString &message);

signals:
    void newStatusMessage(const QString &message,
                          QObject *sender);
    void disableQrc(QObject *object);
};

#endif // CCFGLOBAL_H
