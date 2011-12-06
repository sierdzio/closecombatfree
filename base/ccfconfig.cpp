#include "ccfconfig.h"
//#include <QDebug>

CcfConfig::CcfConfig(const QString &configFilePath, QObject *parent) :
    QObject(parent), CcfError(), filePath(configFilePath)
{
    configuration = new QMap<QString, QPair<QString, bool> >();
    parser = new CcfConfigParser(filePath, this);
    m_terrainInfoMode = "OFF";

    if (!parser->isErrorState()) {
        configuration = parser->configuration();
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
        configuration->remove("uimode");
        configuration->insert("uimode", QPair<QString, bool>(mode, true));
        statusMessage("Ui mode changed to: " + mode);
        emit uiModeChanged();
    } else if (mode == "mobile") {
        mode = "desktop";
        configuration->remove("uimode");
        configuration->insert("uimode", QPair<QString, bool>(mode, true));
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
    return configuration->value("width").first.toInt();
}

int CcfConfig::configWindowHeight()
{
    return configuration->value("height").first.toInt();
}

bool CcfConfig::saveConfig()
{
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

    QChar temp = character.toLower();

    if (temp == 'a') {
        result = Qt::Key_A;
    } else if (temp == 'b') {
        result = Qt::Key_B;
    } else if (temp == 'c') {
        result = Qt::Key_C;
    } else if (temp == 'd') {
        result = Qt::Key_D;
    } else if (temp == 'e') {
        result = Qt::Key_E;
    } else if (temp == 'f') {
        result = Qt::Key_F;
    } else if (temp == 'g') {
        result = Qt::Key_G;
    } else if (temp == 'h') {
        result = Qt::Key_H;
    } else if (temp == 'i') {
        result = Qt::Key_I;
    } else if (temp == 'j') {
        result = Qt::Key_J;
    } else if (temp == 'k') {
        result = Qt::Key_K;
    } else if (temp == 'l') {
        result = Qt::Key_L;
    } else if (temp == 'm') {
        result = Qt::Key_M;
    } else if (temp == 'n') {
        result = Qt::Key_N;
    } else if (temp == 'o') {
        result = Qt::Key_O;
    } else if (temp == 'p') {
        result = Qt::Key_P;
    } else if (temp == 'q') {
        result = Qt::Key_Q;
    } else if (temp == 'r') {
        result = Qt::Key_R;
    } else if (temp == 's') {
        result = Qt::Key_S;
    } else if (temp == 't') {
        result = Qt::Key_T;
    } else if (temp == 'u') {
        result = Qt::Key_U;
    } else if (temp == 'w') {
        result = Qt::Key_W;
    } else if (temp == 'x') {
        result = Qt::Key_X;
    } else if (temp == 'y') {
        result = Qt::Key_Y;
    } else if (temp == 'z') {
        result = Qt::Key_Z;
    } else if (temp == '[') {
        result = Qt::Key_BracketLeft;
    } else if (temp == ']') {
        result = Qt::Key_BracketRight;
    } else if (temp == ';') {
        result = Qt::Key_Semicolon;
    } else if (temp == '\'') {
        result = Qt::Key_Apostrophe;
    } else if (temp == ',') {
        result = Qt::Key_Comma;
    } else if (temp == '.') {
        result = Qt::Key_Period;
    } else if (temp == '/') {
        result = Qt::Key_Slash;
    } else if (temp == '\\') {
        result = Qt::Key_Backslash;
    } else if (temp == '-') {
        result = Qt::Key_Minus;
    } else if (temp == '=') {
        result = Qt::Key_Equal;
    }

    return result;
}

void CcfConfig::windowResized(QSize newSize)
{
    if (configWindowWidth() != newSize.width()) {
        configuration->remove("width");
        configuration->insert("width", QPair<QString, bool>(QString::number(newSize.width()), true));
        emit configWindowWidthChanged();
    }

    if (configWindowHeight() != newSize.height()) {
        configuration->remove("height");
        configuration->insert("height", QPair<QString, bool>(QString::number(newSize.height()), true));
        emit configWindowHeightChanged();
    }
}

void CcfConfig::statusMsg(const QString &message)
{
    statusMessage(message);
}

void CcfConfig::statusMessage(const QString &message)
{
//    qDebug() << this->sender();
    emit newStatusMessage(message, this->sender()); // or QObject::sender()
}

void CcfConfig::setTerrainImageUrl(const QString &url, int width, int height)
{
    QImage tempImage(url);
    terrainImage = new QImage(tempImage.scaled(QSize(width, height)));
//    qDebug() << "Got terrain info in C++. Height:" << terrainImage->width()
//             << ". Height:" << terrainImage->height();
}

int CcfConfig::terrainPixelInfo(int x, int y)
{
    QRgb result(terrainImage->pixel(QPoint(x, y)));
    return qRed(result) + qGreen(result) + qBlue(result);
}
