#ifndef CCFERROR_H
#define CCFERROR_H

#include <QString>

class CcfError
{
public:
    CcfError();
    bool isErrorState();
    QString errorMessage();
    void enterErrorState(const QString &errorMessage);

private:
    bool m_errorState;
    QString m_errorMessage;
};

#endif // CCFERROR_H
