#ifndef CCFCONFIGSAVER_H
#define CCFCONFIGSAVER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QPair>
#include <QFile>
#include "../ccferror.h"

class CcfConfigSaver : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit CcfConfigSaver(QMap<QString, QPair<QString, bool> > *configuration,
                            QList<QString> *configIndexes,
                            const QString &configFilePath,
                            QObject *parent = 0);
    void updateConfigFile();

private:
    QMap<QString, QPair<QString, bool> > *m_configuration;
    QList<QString> *m_configIndexes;
    QString m_configFilePath;
};

#endif // CCFCONFIGSAVER_H
