#ifndef CCFCONFIGPARSER_H
#define CCFCONFIGPARSER_H

#include <QObject>
#include <QList>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QIODevice>
#include <QFile>
#include <QPair>
#include "ccferror.h"

class CcfConfigParser : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit CcfConfigParser(const QString &configFilePath, QObject *parent = 0);
    QMap<QString, QPair<QString, bool> > *configuration();
    QList<QString> *configIndexes();

private:
    void parse(const QString &configFilePath);
    bool readLine(int lineNumber, const QString &line);

    QMap<QString, QPair<QString, bool> > *m_configuration;
    QList<QString> *m_configIndexes;
};

#endif // CCFCONFIGPARSER_H
