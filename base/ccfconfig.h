#ifndef CCFCONFIG_H
#define CCFCONFIG_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QSize>
#include <QPair>
////
#include <QImage>
#include <QPoint>
#include <QSize>
#include <QColor>
#include <QDebug>
////
#include "ccferror.h"
#include "ccfconfigparser.h"
#include "ccfconfigsaver.h"

class CcfConfig : public QObject, public CcfError
{
    Q_OBJECT
    Q_PROPERTY(QString uiMode READ uiMode NOTIFY uiModeChanged)
    Q_PROPERTY(QString terrainInfoMode READ terrainInfoMode NOTIFY uiModeChanged)
    Q_PROPERTY(int configWindowWidth READ configWindowWidth NOTIFY configWindowWidthChanged)
    Q_PROPERTY(int configWindowHeight READ configWindowHeight NOTIFY configWindowHeightChanged)

public:
    explicit CcfConfig(const QString &configFilePath, QObject *parent = 0);

    Q_INVOKABLE QString configurationString();
    Q_INVOKABLE int keyForFunction(const QString &functionName);
    Q_INVOKABLE void toggleUiMode();
    Q_INVOKABLE void toggleTerrainInfoMode();
    Q_INVOKABLE void setTerrainImageUrl(const QString &url, int width, int height);
    Q_INVOKABLE int terrainPixelInfo(int x, int y);

    QString uiMode();
    QString terrainInfoMode();
    int configWindowWidth();
    int configWindowHeight();

    bool saveConfig();

public slots:
    void windowResized(QSize newSize);
    void statusMsg(const QString &message);
    void statusMessage(const QString &message);

signals:
    void configWindowWidthChanged();
    void configWindowHeightChanged();
    void uiModeChanged();
    void terrainInfoModeChanged();
    void newStatusMessage(const QString &message,
                     QObject *sender);

private:
    int findQtKey(QChar character);

    QString m_terrainInfoMode;
    QString filePath;
    QImage *terrainImage;
    CcfConfigParser *parser;
    CcfConfigSaver *saver;
    QMap<QString, QPair<QString, bool> > *configuration;
};

#endif // CCFCONFIG_H
