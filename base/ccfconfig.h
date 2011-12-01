#ifndef CCFCONFIG_H
#define CCFCONFIG_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QSize>
#include <QPair>
#include "ccferror.h"
#include "ccfconfigparser.h"
#include "ccfconfigsaver.h"

class CcfConfig : public QObject, public CcfError
{
    Q_OBJECT
    Q_PROPERTY(QString uiMode READ uiMode NOTIFY uiModeChanged)
    Q_PROPERTY(int configWindowWidth READ configWindowWidth NOTIFY configWindowWidthChanged)
    Q_PROPERTY(int configWindowHeight READ configWindowHeight NOTIFY configWindowHeightChanged)

public:
    explicit CcfConfig(const QString &configFilePath, QObject *parent = 0);

    Q_INVOKABLE QString configurationString();
    Q_INVOKABLE int keyForFunction(const QString &functionName);
    Q_INVOKABLE void toggleUiMode();

    QString uiMode();
    int configWindowWidth();
    int configWindowHeight();

    bool saveConfig();

public slots:
    void windowResized(QSize newSize);

signals:
    void configWindowWidthChanged();
    void configWindowHeightChanged();
    void uiModeChanged();

private:
    int findQtKey(QChar character);

    QString filePath;
    CcfConfigParser *parser;
    CcfConfigSaver *saver;
    QMap<QString, QPair<QString, bool> > *configuration;
};

#endif // CCFCONFIG_H
