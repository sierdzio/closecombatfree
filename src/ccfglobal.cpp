#include "ccfglobal.h"

CcfGlobal::CcfGlobal(QObject *parent) :
    QObject(parent), CcfError()
{
}

void CcfGlobal::statusMsg(const QString &message)
{
    statusMessage(message);
}

void CcfGlobal::statusMessage(const QString &message)
{
    emit newStatusMessage(message, this->sender());
}

void CcfGlobal::disableQrcUse(QObject *object)
{
    emit disableQrc(object);
}
