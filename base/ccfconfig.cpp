#include "ccfconfig.h"

CcfConfig::CcfConfig(const QString &configFilePath, QObject *parent) :
    QObject(parent), CcfError(), filePath(configFilePath)
{
    configuration = new QMap<QString, QString>();
    parser = new CcfConfigParser(filePath, this);

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
            result.append(key + " " + configuration->value(key) + "\n");
        }
    }
    return result;
}

QString CcfConfig::uiMode()
{
    return configuration->value("uimode").toUpper();
}

int CcfConfig::configWindowWidth()
{
    return configuration->value("width").toInt();
}

int CcfConfig::configWindowHeight()
{
    return configuration->value("height").toInt();
}

int CcfConfig::keyForFunction(const QString &functionName)
{
    int result = -1;

    if (configuration->contains(functionName.toLower())) {
        QChar key = configuration->value(functionName.toLower()).at(0);
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
    configuration->remove("width");
    configuration->insert("width", QString::number(newSize.width()));
    emit configWindowWidthChanged();

    configuration->remove("height");
    configuration->insert("height", QString::number(newSize.height()));
    emit configWindowHeightChanged();
}
