#ifndef CCFCONFIGPARSER_H
#define CCFCONFIGPARSER_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QIODevice>
#include <QFile>
#include "ccferror.h"

class CcfConfigParser : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit CcfConfigParser(const QString &configFilePath, QObject *parent = 0);
    QMap<QString, QString> *configuration();

private:
    void parse(const QString &configFilePath);
    bool readLine(const QString &line);

    QMap<QString, QString> *m_configuration;
};

#endif // CCFCONFIGPARSER_H
