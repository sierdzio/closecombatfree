#ifndef CCFCONFIG_H
#define CCFCONFIG_H

#include <QObject>
#include <QString>
#include <QMap>
#include "ccferror.h"
#include "ccfconfigparser.h"

class CcfConfig : public QObject, public CcfError
{
    Q_OBJECT
    Q_PROPERTY(QString uiMode READ uiMode)
    Q_PROPERTY(int configWindowWidth READ configWindowWidth)
    Q_PROPERTY(int configWindowHeight READ configWindowHeight)

public:
    explicit CcfConfig(const QString &configFilePath, QObject *parent = 0);

    Q_INVOKABLE QString configurationString();
    Q_INVOKABLE int keyForFunction(const QString &functionName);

    QString uiMode();
    int configWindowWidth();
    int configWindowHeight();

private:
    int findQtKey(QChar character);

    QString filePath;
    CcfConfigParser *parser;
    QMap<QString, QString> *configuration;
};

#endif // CCFCONFIG_H
