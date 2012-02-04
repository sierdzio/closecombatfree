/****************************************************************************
** Close Combat Free is a free, easily moddable CC-like game.
** Copyright (C) 2011 Tomasz Siekierda
** Email: sierdzio@gmail.com, website: http://www.sierdzio.com
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program (in doc/ directory).
** If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
****************************************************************************/

#ifndef CCFCONFIG_H
#define CCFCONFIG_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QPair>
#include <QStringList>
#include <QKeySequence>
#include <QSize>
#include "../ccferror.h"
#include "../ccfglobal.h"
#include "ccfconfigparser.h"
#include "ccfconfigsaver.h"

#include <QDebug>

/*!
  This class is responsible for providing and modifying configuration
  to QML. It also loads config on init, and saves on exit (using CcfConfigParser
  and CcfConfigSaver).
  */
class CcfConfig : public QObject, public CcfError
{
    Q_OBJECT
    Q_PROPERTY(QString uiMode READ uiMode WRITE setUiMode NOTIFY uiModeChanged)
    Q_PROPERTY(QString terrainInfoMode READ terrainInfoMode NOTIFY uiModeChanged)
    Q_PROPERTY(int windowWidth READ windowWidth WRITE setWindowWidth NOTIFY windowWidthChanged)
    Q_PROPERTY(int windowHeight READ windowHeight WRITE setWindowHeight NOTIFY windowHeightChanged)
    Q_PROPERTY(bool maximised READ maximised WRITE setMaximised NOTIFY maximisedChanged)
    Q_PROPERTY(bool rememberDimensions READ rememberDimensions WRITE setRememberDimensions NOTIFY rememberDimensionsChanged)

public:
    explicit CcfConfig(const QString &configFilePath, CcfGlobal *globalObject,
                       QObject *parent = 0);

    Q_INVOKABLE QString configurationString();
    Q_INVOKABLE int keyForFunction(const QString &functionName);
    Q_INVOKABLE void toggleUiMode();
    Q_INVOKABLE void toggleTerrainInfoMode();
    Q_INVOKABLE void setUiMode(const QString &newMode);
    Q_INVOKABLE void setWindowWidth(int width);
    Q_INVOKABLE void setWindowHeight(int height);
    // For use when not remembering dimensions on exit
    Q_INVOKABLE void forceSetWindowWidth(int width);
    Q_INVOKABLE void forceSetWindowHeight(int height);

    Q_INVOKABLE bool maximised();
    Q_INVOKABLE void setMaximised(bool newValue);
    Q_INVOKABLE bool rememberDimensions();
    Q_INVOKABLE void setRememberDimensions(bool newValue);
    // Shortcuts list loading and saving (in-game preferences menu)
    // This would work better if done with Qt MVC!
    Q_INVOKABLE QStringList shortcutNamesList();
    Q_INVOKABLE QStringList shortcutValuesList();
    Q_INVOKABLE void setShortcut(const QString &option, const QString &value);
    QString uiMode();
    QString terrainInfoMode();
    int windowWidth();
    int windowHeight();
    bool saveConfig();

public slots:
    void windowResized(QSize newSize);

signals:
    void windowWidthChanged();
    void windowHeightChanged();
    void uiModeChanged();
    void terrainInfoModeChanged();
    void maximisedChanged();
    void maximise();
    void demaximise();
    void rememberDimensionsChanged();
    void sizeModifiedInGame(int width, int height);

private:
    int findQtKey(QChar character);
    bool stringToBool(const QString &stringToConvert);
    QString boolToString(bool boolToConvert);
    void replaceElement(const QString &elementToReplace, const QString &newValue);
    void parseValidKeyboardShortcuts();

    int runtimeWidth, runtimeHeight;
    QString m_terrainInfoMode;
    QString filePath;
    CcfGlobal *global;
    CcfConfigParser *parser;
    CcfConfigSaver *saver;
    QMap<QString, QPair<QString, bool> > *configuration;
    QMap<QString, QString> keyboardShortcuts;
};

#endif // CCFCONFIG_H
