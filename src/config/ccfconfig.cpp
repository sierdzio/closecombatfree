#include "ccfconfig.h"

CcfConfig::CcfConfig(const QString &configFilePath, CcfGlobal *globalObject, QObject *parent) :
    QObject(parent), CcfError(), filePath(configFilePath), global(globalObject)
{
    configuration = new QMap<QString, QPair<QString, bool> >();
    parser = new CcfConfigParser(filePath, this);
    m_terrainInfoMode = "OFF";

    if (!parser->isErrorState()) {
        configuration = parser->configuration();
        runtimeWidth = configuration->value("width").first.toInt();
        runtimeHeight = configuration->value("height").first.toInt();
        parseValidKeyboardShortcuts();
    } else {
        enterErrorState(parser->errorMessage());
    }
}

QString CcfConfig::configurationString()
{
    QString result;
    if (!isErrorState()) {
        foreach (const QString &key, configuration->keys()) {
            result.append(key + " " + configuration->value(key).first
                          + ", " + configuration->value(key).second + "\n");
        }
    }
    return result;
}

QString CcfConfig::uiMode()
{
    return configuration->value("uimode").first.toUpper();
}

void CcfConfig::toggleUiMode()
{
    QString mode = configuration->value("uimode").first;

    if (mode == "desktop") {
        mode = "mobile";
        replaceElement("uimode", mode);
        global->statusMessage("Ui mode changed to: " + mode);
        emit uiModeChanged();
    } else if (mode == "mobile") {
        mode = "desktop";
        replaceElement("uimode", mode);
        global->statusMessage("Ui mode changed to: " + mode);
        emit uiModeChanged();
    }
}

QString CcfConfig::terrainInfoMode()
{
    return m_terrainInfoMode;
}

void CcfConfig::toggleTerrainInfoMode()
{
    if (m_terrainInfoMode == "ON") {
        m_terrainInfoMode = "OFF";
    } else {
        m_terrainInfoMode = "ON";
    }
    emit terrainInfoModeChanged();
}

int CcfConfig::windowWidth()
{
    return runtimeWidth;
}

int CcfConfig::windowHeight()
{
    return runtimeHeight;
}

bool CcfConfig::saveConfig()
{
    // Put screen size checks here!
    if (configuration->value("remember dimensions on exit").first == "true") {
        if (configuration->value("height").first.toInt() != runtimeHeight) {
            replaceElement("height", QString::number(runtimeHeight));
        }
        if (configuration->value("width").first.toInt() != runtimeWidth) {
            replaceElement("width", QString::number(runtimeWidth));
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

int CcfConfig::keyForFunction(const QString &functionName)
{
    int result = -1;

    if (configuration->contains(functionName.toLower())) {
        QChar key = configuration->value(functionName.toLower()).first.at(0);
        result = findQtKey(key);
    }

    return result;
}

int CcfConfig::findQtKey(QChar character)
{
    int result = -1;

    QKeySequence key(character);
    result = key[0];

    return result;
}

bool CcfConfig::stringToBool(const QString &stringToConvert)
{
    if (stringToConvert == "true") {
        return true;
    } else if (stringToConvert == "false") {
        return false;
    }
    return false;
}

QString CcfConfig::boolToString(bool boolToConvert)
{
    if (boolToConvert == true) {
        return "true";
    } else if (boolToConvert == false) {
        return "false";
    }
    return QString();
}

void CcfConfig::replaceElement(const QString &elementToReplace, const QString &newValue)
{
    configuration->remove(elementToReplace);
    configuration->insert(elementToReplace, QPair<QString, bool>(newValue, true));
}

void CcfConfig::parseValidKeyboardShortcuts()
{
    foreach (QString key, configuration->keys()) {
        // This will probably not work well.
        // Some additional parsing is needed.
        QKeySequence valueToCheck = configuration->value(key).first;
        QString value = valueToCheck.toString().toLower();
        if (value != "") {
            keyboardShortcuts.insert(key, value);
        }
    }
}

void CcfConfig::windowResized(QSize newSize)
{
    if (windowWidth() != newSize.width()
            || windowHeight() != newSize.height()) {

        runtimeHeight = newSize.height();
        runtimeWidth = newSize.width();
        emit windowWidthChanged();
        emit windowHeightChanged();
    }
}

void CcfConfig::setUiMode(const QString &newMode)
{
    if (newMode.toLower() != configuration->value("uimode").first) {
        if (newMode.toLower() == "desktop") {
            replaceElement("uimode", "desktop");
            global->statusMessage("Ui mode changed to: desktop");
            emit uiModeChanged();
        } else if (newMode.toLower() == "mobile") {
            replaceElement("uimode", "mobile");
            global->statusMessage("Ui mode changed to: mobile");
            emit uiModeChanged();
        }
    }
}

void CcfConfig::setWindowHeight(int height)
{
    runtimeHeight = height;
    emit windowWidthChanged();
}

void CcfConfig::setWindowWidth(int width)
{
    runtimeWidth = width;
    emit windowHeightChanged();
}

void CcfConfig::forceSetWindowWidth(int width)
{
    replaceElement("width", QString::number(width));
}

void CcfConfig::forceSetWindowHeight(int height)
{
    replaceElement("height", QString::number(height));
}

bool CcfConfig::maximised()
{
    QString result = configuration->value("maximised").first;
    return stringToBool(result);
}

void CcfConfig::setMaximised(bool newValue)
{
    QString current = configuration->value("maximised").first;
    bool currentBool = stringToBool(current);

    if (currentBool != newValue) {
        replaceElement("maximised", boolToString(newValue));
        emit maximisedChanged();
        if (newValue)
            emit maximise();
        else
            emit demaximise();
    }
}

bool CcfConfig::rememberDimensions()
{
    QString result = configuration->value("remember dimensions on exit").first;
    return stringToBool(result);
}

void CcfConfig::setRememberDimensions(bool newValue)
{
    QString current = configuration->value("remember dimensions on exit").first;
    bool currentBool = stringToBool(current);

    if (currentBool != newValue) {
        replaceElement("remember dimensions on exit", boolToString(newValue));
        emit rememberDimensionsChanged();
    }
}

QStringList CcfConfig::shortcutNamesList()
{
    return keyboardShortcuts.keys();
}

QStringList CcfConfig::shortcutValuesList()
{
    return keyboardShortcuts.values();
}

void CcfConfig::setShortcut(const QString &option, const QString &value)
{
    QString lowOption = option.toLower();
    if (keyboardShortcuts.contains(lowOption)
            && (value != keyboardShortcuts.value(lowOption))
            && (QKeySequence(value).toString() != "")) {
        replaceElement(lowOption, value);
    }
}
