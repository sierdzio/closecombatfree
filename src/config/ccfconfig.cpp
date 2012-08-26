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

#include <QtGui/QKeySequence>

#include "ccfconfig.h"

/*!
  Initialises the config and important variables, used later in QML.

  Currently uses global object in UI mode changing. This is wrong, should and will
  be fixed.
  */
CcfConfig::CcfConfig(const QString &configFilePath, CcfGlobal *globalObject, QObject *parent) :
    QObject(parent), CcfError(), filePath(configFilePath), global(globalObject)
{
    configuration = new CcfConfigData();
    parser = new CcfConfigParser(filePath, this);
    m_terrainInfoMode = "OFF";

    if (!parser->isErrorState()) {
        configuration = parser->configuration();
        runtimeWidth = configuration->value("width").toInt();
        runtimeHeight = configuration->value("height").toInt();
        parseValidKeyboardShortcuts();
    } else {
        enterErrorState(parser->errorMessage());
    }
}

/*!
  A convenience method returning nicely formatted string with config brief.
  */
QString CcfConfig::configurationString() const
{
    QString result;
    if (!isErrorState()) {
        result = configuration->toString();
    }
    return result;
}

/*!
  Getter for UI mode (desktop or mobile).

  \sa toggleUiMode, setUiMode
  */
QString CcfConfig::getUiMode() const
{
    return configuration->value("uimode").toUpper();
}

/*!
  Toggles the UI mode (desktop => mobile and vice versa).

  \sa uiMode, setUiMode
  */
void CcfConfig::toggleUiMode()
{
    QString mode = configuration->value("uimode");

    if (mode == "desktop") {
        mode = "mobile";
        configuration->replace("uimode", mode);
        global->statusMessage("Ui mode changed to: " + mode);
        emit uiModeChanged();
    } else if (mode == "mobile") {
        mode = "desktop";
        configuration->replace("uimode", mode);
        global->statusMessage("Ui mode changed to: " + mode);
        emit uiModeChanged();
    }
}

/*!
  Returns the status of terrain information mode.

  Information mode can be off (default) - in that case info on terrain can
  be displayed by a long right mouse click on any place on map.
  If it is on, then a long left click (or finger tap) triggers the information
  to be shown.

  \sa toggleTerrainInfoMode
  */
QString CcfConfig::getTerrainInfoMode() const
{
    return m_terrainInfoMode;
}

/*!
  Toggles terrain info mode.

  \sa terrainInfoMode
  */
void CcfConfig::toggleTerrainInfoMode()
{
    if (m_terrainInfoMode == "ON") {
        m_terrainInfoMode = "OFF";
    } else {
        m_terrainInfoMode = "ON";
    }
    emit terrainInfoModeChanged();
}

/*!
  Returns game window's width.
  */
int CcfConfig::getWindowWidth() const
{
    return runtimeWidth;
}

/*!
  Returns game window's height.
  */
int CcfConfig::getWindowHeight() const
{
    return runtimeHeight;
}

/*!
  Triggers configuration save (to "config" and "config_old" files).
  */
bool CcfConfig::saveConfig()
{
    // Put screen size checks here!
    if (configuration->value("remember dimensions on exit") == "true") {
        if (configuration->value("height").toInt() != runtimeHeight) {
            configuration->replace("height", QString::number(runtimeHeight));
        }
        if (configuration->value("width").toInt() != runtimeWidth) {
            configuration->replace("width", QString::number(runtimeWidth));
        }
    }


    saver = new CcfConfigSaver(configuration, parser->configIndexes(),
                               filePath, this);
    saver->updateConfigFile();

    if (saver->isErrorState()) {
        enterErrorState("Saving config file failed. Error: " + saver->errorMessage());
        return false;
    }

    return true;
}

/*!
  Returns key number for a given \a functionName.
  */
int CcfConfig::keyForFunction(const QString &functionName) const
{
    int result = -1;

    if (configuration->contains(functionName.toLower())) {
        QChar key = configuration->value(functionName.toLower()).at(0);
        result = findQtKey(key);
    }

    return result;
}

/*!
  Finds Qt key number for a given \a character.
  */
int CcfConfig::findQtKey(QChar character) const
{
    int result = -1;

    QKeySequence key(character);
    result = key[0];

    return result;
}

/*!
  A simple converter from string to bool.

  Converts "true" into boolean true (1), and "false" into boolean false (0).

  \sa boolToString
  */
bool CcfConfig::stringToBool(const QString &stringToConvert) const
{
    if (stringToConvert == "true") {
        return true;
    } else if (stringToConvert == "false") {
        return false;
    }
    return false;
}

/*!
  A simple bool to QString converter.

  Returns "true" for true, and "false" for false.

  \sa stringToBool
  */
QString CcfConfig::boolToString(bool boolToConvert) const
{
    if (boolToConvert == true) {
        return "true";
    } else if (boolToConvert == false) {
        return "false";
    }
    return QString();
}

/*!
  A basic parser-validator for key sequences found in config file.
  */
void CcfConfig::parseValidKeyboardShortcuts()
{
    for (int i = 0; i < configuration->size(); ++i) {
        // This will probably not work well.
        // Some additional parsing is needed.
        QKeySequence valueToCheck(configuration->value(i));
        QString value = valueToCheck.toString().toLower();
        if (value != "") {
            keyboardShortcuts.insert(configuration->key(i), value);
        }
    }
}

/*!
  Updates runtime window dimensions, and emits relevant signals to propagate\
  that information.

  \sa setWindowHeight, setWindowWidth, forceSetWindowWidth, forceSetWindowHeight
  */
void CcfConfig::windowResized(QSize newSize)
{
    if (getWindowWidth() != newSize.width()
            || getWindowHeight() != newSize.height()) {

        runtimeHeight = newSize.height();
        runtimeWidth = newSize.width();
        emit windowWidthChanged();
        emit windowHeightChanged();
    }
}

/*!
  Sets uiMode to a given value.

  Possible options are:
  \li desktop
  \li mobile

  If \a newMode is spelled incorrectly, nothing is changed.

  \sa uiMode, toggleUiMode
  */
void CcfConfig::setUiMode(const QString &newMode)
{
    if (newMode.toLower() != configuration->value("uimode")) {
        if (newMode.toLower() == "desktop") {
            configuration->replace("uimode", "desktop");
            global->statusMessage("Ui mode changed to: desktop");
            emit uiModeChanged();
        } else if (newMode.toLower() == "mobile") {
            configuration->replace("uimode", "mobile");
            global->statusMessage("Ui mode changed to: mobile");
            emit uiModeChanged();
        }
    }
}

/*!
  Sets window height to a new \a height.

  \sa windowResized, setWindowWidth, forceSetWindowWidth, forceSetWindowHeight
  */
void CcfConfig::setWindowHeight(int height)
{
    runtimeHeight = height;
    emit windowWidthChanged();
}

/*!
  Sets window width to a new \a width.

  \sa setWindowHeight, windowResized, forceSetWindowWidth, forceSetWindowHeight
  */
void CcfConfig::setWindowWidth(int width)
{
    runtimeWidth = width;
    emit windowHeightChanged();
}

/*!
  Forces width change in the configuration file.

  \sa setWindowHeight, setWindowWidth, windowResized, forceSetWindowHeight
  */
void CcfConfig::forceSetWindowWidth(int width)
{
    configuration->replace("width", QString::number(width));
}

/*!
  Forces height change in the configuration file.

  \sa setWindowHeight, setWindowWidth, windowResized, forceSetWindowWidth
  */
void CcfConfig::forceSetWindowHeight(int height)
{
    configuration->replace("height", QString::number(height));
}

/*!
  Returns true is configuration says that window should be maximised.

  \sa setMaximised
  */
bool CcfConfig::isMaximised() const
{
    QString result = configuration->value("maximised");
    return stringToBool(result);
}

/*!
  Sets maximised configuration value to \a newValue.

  \sa maximised
  */
void CcfConfig::setMaximised(bool newValue)
{
//    QString current = configuration->value("maximised").first;
//    bool currentBool = stringToBool(current);
    bool currentBool = isMaximised();

    if (currentBool != newValue) {
        configuration->replace("maximised", boolToString(newValue));
        emit maximisedChanged();
        if (newValue)
            emit maximise();
        else
            emit demaximise();
    }
}

/*!
  Returns true if rememberDimensions value is checked in configuration.

  \sa setRememberDimensions
  */
bool CcfConfig::isRememberDimensionsSet() const
{
    QString result = configuration->value("remember dimensions on exit");
    return stringToBool(result);
}

/*!
  Sets rememberDimensions value in configuration to \a newValue.

  \sa rememberDimensions
  */
void CcfConfig::setRememberDimensions(bool newValue)
{
    QString current = configuration->value("remember dimensions on exit");
    bool currentBool = stringToBool(current);

    if (currentBool != newValue) {
        configuration->replace("remember dimensions on exit", boolToString(newValue));
        emit rememberDimensionsChanged();
    }
}

/*!
  Returns a list of human-readable names for shortcuts.

  \sa shortcutValuesList
  */
QStringList CcfConfig::shortcutNamesList() const
{
    return keyboardShortcuts.keys();
}

/*!
  Returns a list of shortcutsthemselves (in form of strings).

  \sa shortcutNamesList
  */
QStringList CcfConfig::shortcutValuesList() const
{
    return keyboardShortcuts.values();
}

/*!
  Sets a shortcut \a option to new \a value.
  */
void CcfConfig::setShortcut(const QString &option, const QString &value)
{
    QString lowOption = option.toLower();
    if (keyboardShortcuts.contains(lowOption)
            && (value != keyboardShortcuts.value(lowOption))
            && (QKeySequence(value).toString() != "")) {
        configuration->replace(lowOption, value);
    }
}
