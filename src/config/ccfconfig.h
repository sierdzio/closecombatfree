#ifndef CCFCONFIG_H
#define CCFCONFIG_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QSize>
#include <QPair>

#include <QImage>
#include <QPoint>
#include <QSize>
#include <QColor>
#include <QDebug>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QFileInfo>
//#include <QKeyEvent>
#include <QKeySequence>
////
#include <QTextStream>
#include <QDeclarativeListReference>
////
#include "../ccferror.h"
#include "ccfconfigparser.h"
#include "ccfconfigsaver.h"

class CcfConfig : public QObject, public CcfError
{
    Q_OBJECT
    Q_PROPERTY(QString uiMode READ uiMode WRITE setUiMode NOTIFY uiModeChanged)
    Q_PROPERTY(QString terrainInfoMode READ terrainInfoMode NOTIFY uiModeChanged)
    Q_PROPERTY(int configWindowWidth READ configWindowWidth WRITE setConfigWindowWidth NOTIFY configWindowWidthChanged)
    Q_PROPERTY(int configWindowHeight READ configWindowHeight WRITE setConfigWindowHeight NOTIFY configWindowHeightChanged)
    Q_PROPERTY(bool configMaximised READ configMaximised WRITE setConfigMaximised NOTIFY configMaximisedChanged)
    Q_PROPERTY(bool configRememberDimensions READ configRememberDimensions WRITE setConfigRememberDimensions NOTIFY configRememberDimensionsChanged)

public:
    explicit CcfConfig(const QString &configFilePath, QObject *parent = 0);

    Q_INVOKABLE QString configurationString();
    Q_INVOKABLE int keyForFunction(const QString &functionName);
    Q_INVOKABLE void toggleUiMode();
    Q_INVOKABLE void toggleTerrainInfoMode();
    Q_INVOKABLE void setTerrainImageUrl(const QString &url, int width, int height);
    Q_INVOKABLE int terrainPixelInfo(int x, int y);    
    Q_INVOKABLE QStringList scenariosList();
    Q_INVOKABLE QString scenarioPath(int index);

    Q_INVOKABLE void setUiMode(const QString &newMode);
    Q_INVOKABLE void setConfigWindowWidth(int width);
    Q_INVOKABLE void setConfigWindowHeight(int height);
    // For use when not remembering dimensions on exit
    Q_INVOKABLE void forceSetConfigWindowWidth(int width);
    Q_INVOKABLE void forceSetConfigWindowHeight(int height);

    Q_INVOKABLE bool configMaximised();
    Q_INVOKABLE void setConfigMaximised(bool newValue);
    Q_INVOKABLE bool configRememberDimensions();
    Q_INVOKABLE void setConfigRememberDimensions(bool newValue);
    // Shortcuts list loading and saving (in-game preferences menu)
    // This would work better if done with Qt MVC!
    Q_INVOKABLE QStringList configShortcutNamesList();
    Q_INVOKABLE QStringList configShortcutValuesList();
    Q_INVOKABLE void setConfigShortcut(const QString &option, const QString &value);
    ////
    // Additional ideas:
    //  - might add map damages info
    Q_INVOKABLE void saveGame(const QDeclarativeListReference &unitsList,
                              const QString &mapFile,
                              const QString &saveFileName = "saves/save1.qml");
//    Q_INVOKABLE QString loadGame();
    Q_INVOKABLE QStringList savedGamesList();
    Q_INVOKABLE void disableQrcUse(QObject *object);
    ////
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
    ////
    void configMaximisedChanged();
    void configMaximise();
    void configDemaximise();
    void configRememberDimensionsChanged();
    void sizeModifiedInGame(int width, int height);
    void disableQrc(QObject *object);
    ////

private:
    int findQtKey(QChar character);
    bool stringToBool(const QString &stringToConvert);
    QString boolToString(bool boolToConvert);
    void replaceElement(const QString &elementToReplace, const QString &newValue);
    void parseValidKeyboardShortcuts();
    QString addSavePropertyIfExists(const QObject *object, const QString &propertyName, bool useQuotes = false);

    int runtimeWidth, runtimeHeight;

    QStringList m_scenariosList;
    QString m_terrainInfoMode;
    QString filePath;
    QString tab;
    QImage *terrainImage;
    CcfConfigParser *parser;
    CcfConfigSaver *saver;
    QMap<QString, QPair<QString, bool> > *configuration;
    QMap<QString, QString> keyboardShortcuts;
};

#endif // CCFCONFIG_H
