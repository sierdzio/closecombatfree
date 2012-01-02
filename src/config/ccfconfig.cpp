#include "ccfconfig.h"

CcfConfig::CcfConfig(const QString &configFilePath, QObject *parent) :
    QObject(parent), CcfError(), filePath(configFilePath)
{
    configuration = new QMap<QString, QPair<QString, bool> >();
    parser = new CcfConfigParser(filePath, this);
    m_terrainInfoMode = "OFF";
    tab = "    ";

    QDir scenarioDir(":/scenarios");
    m_scenariosList = scenarioDir.entryList();

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
        statusMessage("Ui mode changed to: " + mode);
        emit uiModeChanged();
    } else if (mode == "mobile") {
        mode = "desktop";
        replaceElement("uimode", mode);
        statusMessage("Ui mode changed to: " + mode);
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

int CcfConfig::configWindowWidth()
{
    return runtimeWidth;
}

int CcfConfig::configWindowHeight()
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
    if (configWindowWidth() != newSize.width()
            || configWindowHeight() != newSize.height()) {

        runtimeHeight = newSize.height();
        runtimeWidth = newSize.width();
        emit configWindowWidthChanged();
        emit configWindowHeightChanged();
    }
}

void CcfConfig::statusMsg(const QString &message)
{
    statusMessage(message);
}

void CcfConfig::statusMessage(const QString &message)
{
    emit newStatusMessage(message, this->sender());
}

void CcfConfig::setTerrainImageUrl(const QString &url, int width, int height)
{
    // Hack for QRC support
    QString nUrl = url;
    nUrl.remove(0, 3);
    QImage tempImage(nUrl);
    terrainImage = new QImage(tempImage.scaled(QSize(width, height)));
}

int CcfConfig::terrainPixelInfo(int x, int y)
{
    QRgb result(terrainImage->pixel(QPoint(x, y)));
    return qRed(result) + qGreen(result) + qBlue(result);
}

QStringList CcfConfig::scenariosList()
{
    return m_scenariosList;
}

QString CcfConfig::scenarioPath(int index)
{
    return m_scenariosList.at(index);
}

void CcfConfig::setUiMode(const QString &newMode)
{
    if (newMode.toLower() != configuration->value("uimode").first) {
        if (newMode.toLower() == "desktop") {
            replaceElement("uimode", "desktop");
            statusMessage("Ui mode changed to: desktop");
            emit uiModeChanged();
        } else if (newMode.toLower() == "mobile") {
            replaceElement("uimode", "mobile");
            statusMessage("Ui mode changed to: mobile");
            emit uiModeChanged();
        }
    }
}

void CcfConfig::setConfigWindowHeight(int height)
{
    runtimeHeight = height;
    emit configWindowWidthChanged();
}

void CcfConfig::setConfigWindowWidth(int width)
{
    runtimeWidth = width;
    emit configWindowHeightChanged();
}

void CcfConfig::forceSetConfigWindowWidth(int width)
{
    replaceElement("width", QString::number(width));
}

void CcfConfig::forceSetConfigWindowHeight(int height)
{
    replaceElement("height", QString::number(height));
}

bool CcfConfig::configMaximised()
{
    QString result = configuration->value("maximised").first;
    return stringToBool(result);
}

void CcfConfig::setConfigMaximised(bool newValue)
{
    QString current = configuration->value("maximised").first;
    bool currentBool = stringToBool(current);

    if (currentBool != newValue) {
        replaceElement("maximised", boolToString(newValue));
        emit configMaximisedChanged();
        if (newValue)
            emit configMaximise();
        else
            emit configDemaximise();
    }
}

bool CcfConfig::configRememberDimensions()
{
    QString result = configuration->value("remember dimensions on exit").first;
    return stringToBool(result);
}

void CcfConfig::setConfigRememberDimensions(bool newValue)
{
    QString current = configuration->value("remember dimensions on exit").first;
    bool currentBool = stringToBool(current);

    if (currentBool != newValue) {
        replaceElement("remember dimensions on exit", boolToString(newValue));
        emit configRememberDimensionsChanged();
    }
}

QStringList CcfConfig::configShortcutNamesList()
{
    return keyboardShortcuts.keys();
}

QStringList CcfConfig::configShortcutValuesList()
{
    return keyboardShortcuts.values();
}

void CcfConfig::setConfigShortcut(const QString &option, const QString &value)
{
    QString lowOption = option.toLower();
    if (keyboardShortcuts.contains(lowOption)
            && (value != keyboardShortcuts.value(lowOption))
            && (QKeySequence(value).toString() != "")) {
        replaceElement(lowOption, value);
    }
}

void CcfConfig::saveGame(const QDeclarativeListReference &unitsList, const QString &mapFile, const QString &saveFileName)
{
    // As a first attempt, I will generate the whole file myself.
    // A better approach for the future might be to copy and modify
    // a real scenario file, OR create a QML element like ScenarioLoader
    // which would have "map", "units" properties.

    // Init. Read template.
    QFile templateFile(":/core/config/saveFileTemplate.txt");
    if (!templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal("Template file could not be read! Cannot continue, bailing out.");
        return;
    }

    // File numbers incrementation should go here, or at least overwrite warnings!
    QFile saveFile(saveFileName);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        enterErrorState("Could not save the file: " + saveFileName);
        return;
    }

    QString fileContent = templateFile.readAll();

    // Fill template with given data.
    fileContent.replace("%customImports%", "");
    fileContent.replace("%mapFile%", mapFile);

    // Save units. Hope this will work. If not - convert units list to string list in JS.
    QString units;
    for (int i = 0; i < unitsList.count(); i++) {
        // In release code, this needs to include order queue,
        // soldier states, damages etc.
        QObject *unit = unitsList.at(i);
        units += tab + unit->property("unitFileName").toString() + " {\n"
                + tab + tab + "objectName: \"" + unit->objectName() + "\"\n"
                + tab + tab + "x: " + unit->property("x").toString() + "\n"
                + tab + tab + "y: " + unit->property("y").toString() + "\n"
                + tab + tab + "rotation: " + unit->property("rotation").toString() + "\n";
        units += addSavePropertyIfExists(unit, "turretRotation");
        units += addSavePropertyIfExists(unit, "hullColor", true);
        units += tab + "}\n";
    }
    fileContent.replace("%units%", units);

    QTextStream out(&saveFile);
    out << fileContent;
    saveFile.close();
}

//QString CcfConfig::loadGame()
//{
//}

QStringList CcfConfig::savedGamesList()
{
    QDir saveDir("saves");
    return saveDir.entryList(QDir::Files);
}

void CcfConfig::disableQrcUse(QObject *object)
{
    emit disableQrc(object);
}

QString CcfConfig::addSavePropertyIfExists(const QObject *object, const QString &propertyName, bool useQuotes)
{
    QString result;
    QByteArray ba = propertyName.toLocal8Bit();
    if (object->metaObject()->indexOfProperty(ba.data()) != -1) {
        result = tab + tab + propertyName + ": ";
        if (useQuotes)
            result += "\"";
        result += object->property(ba.data()).toString();
        if (useQuotes)
            result += "\"";
        result += "\n";
    }
    return result;
}
