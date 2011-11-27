#include "ccferror.h"

CcfError::CcfError()
{
    m_errorState = false;
}

bool CcfError::isErrorState()
{
    return m_errorState;
}

QString CcfError::errorMessage()
{
    return m_errorMessage;
}

void CcfError::enterErrorState(const QString &errorMsg)
{
    m_errorState = true;
    m_errorMessage = errorMsg;
}
