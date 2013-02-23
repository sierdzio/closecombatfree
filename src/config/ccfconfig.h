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
#include <QStringList>
#include <QSize>
#include <QMap>

#include "../ccferror.h"
#include "../ccfglobal.h"
#include "ccfconfigparser.h"
#include "ccfconfigsaver.h"
#include "ccfconfigdata.h"

/*!
  \defgroup CloseCombatFree Game code
  @{
 */

/*!
  This class is responsible for providing and modifying configuration
  to QML. It also loads config on init, and saves on exit (using CcfConfigParser
  and CcfConfigSaver).
  */
class CcfConfig : public QObject, public CcfError
{
    Q_OBJECT

    /*!
      Represents the User Interface Mode to be used.

      Choice is: desktop or mobile.
      */
    Q_PROPERTY(QString uiMode READ getUiMode WRITE setUiMode NOTIFY uiModeChanged)

    /*!
      Represents the state of terrain info mode.

      Choice is: on or off.
      */
    Q_PROPERTY(QString terrainInfoMode READ getTerrainInfoMode NOTIFY uiModeChanged)

    /*!
      Represents game window width.
      */
    Q_PROPERTY(int windowWidth READ getWindowWidth WRITE setWindowWidth NOTIFY windowWidthChanged)

    /*!
      Represents game window height.
      */
    Q_PROPERTY(int windowHeight READ getWindowHeight WRITE setWindowHeight NOTIFY windowHeightChanged)

    /*!
      Holds whether window is maximised or not (configuration wise).
      */
    Q_PROPERTY(bool maximised READ isMaximised WRITE setMaximised NOTIFY maximisedChanged)

    /*!
      Boolean property used to determine whether window dimensions should be
      remembered on exit.

      Note: this applies only to game being closed from within the game, and not
      by external means (window manager, process manager etc.).
      */
    Q_PROPERTY(bool rememberDimensions READ isRememberDimensionsSet WRITE setRememberDimensions NOTIFY rememberDimensionsChanged)

public:
    explicit CcfConfig(const QString &configFilePath, CcfGlobal *globalObject,
                       QObject *parent = 0);

    Q_INVOKABLE QString configurationString() const;
    Q_INVOKABLE int keyForFunction(const QString &functionName) const;
    Q_INVOKABLE void toggleUiMode();
    Q_INVOKABLE void toggleTerrainInfoMode();
    Q_INVOKABLE void setUiMode(const QString &newMode);
    Q_INVOKABLE void setWindowWidth(int width);
    Q_INVOKABLE void setWindowHeight(int height);
    Q_INVOKABLE void forceSetWindowWidth(int width);
    Q_INVOKABLE void forceSetWindowHeight(int height);
    Q_INVOKABLE bool isMaximised() const;
    Q_INVOKABLE void setMaximised(bool newValue);
    Q_INVOKABLE bool isRememberDimensionsSet() const;
    Q_INVOKABLE void setRememberDimensions(bool newValue);
    Q_INVOKABLE QStringList shortcutNamesList() const;
    Q_INVOKABLE QStringList shortcutValuesList() const;
    Q_INVOKABLE void setShortcut(const QString &option, const QString &value);
    QString getUiMode() const;
    QString getTerrainInfoMode() const;
    int getWindowWidth() const;
    int getWindowHeight() const;
    bool saveConfig();

public slots:
    void windowResized(QSize newSize);

signals:
    /*!
      Emited when window's width changes.
      */
    void windowWidthChanged();

    /*!
      Emited when window's height changes.
      */
    void windowHeightChanged();

    /*!
      Emited on uiMode change.
      */
    void uiModeChanged();

    /*!
      Emited when terrainInfoMode changes.
      */
    void terrainInfoModeChanged();

    /*!
      Emited when state of maximisation changes.
      */
    void maximisedChanged();

    /*!
      Emited to force app window to maximise.
      */
    void maximise();

    /*!
      Emited to force app window to demaximise.
      */
    void demaximise();

    /*!
      Emited when option to remember dimensions changes.
      */
    void rememberDimensionsChanged();

    /*!
      Emited when game window size changes in-game.
      */
    void sizeModifiedInGame(int width, int height);

private:
    int findQtKey(QChar character) const;
    bool stringToBool(const QString &stringToConvert) const;
    QString boolToString(bool boolToConvert) const;
    void parseValidKeyboardShortcuts();

    int mRuntimeWidth, mRuntimeHeight;
    QString mTerrainInfoMode;
    QString mFilePath;
    CcfGlobal *mGlobal;
    CcfConfigParser *mParser;
    CcfConfigSaver *mSaver;
    CcfConfigData *mConfiguration;
    QMap<QString, QString> mKeyboardShortcuts;
};

/*! @}*/

#endif // CCFCONFIG_H
