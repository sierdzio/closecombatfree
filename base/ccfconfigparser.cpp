#include "ccfconfigparser.h"

CcfConfigParser::CcfConfigParser(const QString &configFilePath, QObject *parent) :
    QObject(parent), CcfError()
{
    m_configuration = new QMap<QString, QPair<QString, bool> >();
    parse(configFilePath);
}

QMap<QString, QPair<QString, bool> > *CcfConfigParser::configuration()
{
    return m_configuration;
}

void CcfConfigParser::parse(const QString &configFilePath)
{
    QFile file(configFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        enterErrorState("Could not open the config file.");
        return;
    }

    while (!file.atEnd()) {
        if (!readLine(file.readLine()))
            return;
    }
}

bool CcfConfigParser::readLine(const QString &lineToParse)
{
    QString line = lineToParse.simplified();

    if (line.isEmpty()) {
        return true;
    }

    // Detect empty lines and comments
    for (int i = 0; i < line.length(); i++) {
        QChar character = line.at(i);
        if (character.isSpace()) {
            continue; // Could be an empty line or a comment
        } else if (character == QChar('#')) {// || (character == QChar('\n')) || (character == QChar('\0'))) {
            return true; // It's a comment
        } else {
            break; // Non comment, non empty line. Continue with parsing.
        }
    }

    QStringList lineData;

    if (line.contains(" = ")) {
        lineData = line.split(" = ");
    } else if (line.contains("=")) {
        lineData = line.split("=");
    } else {
        enterErrorState("Invalid line detected - not empty, not comment, and no data!");
        return false;
    }

    QString key = lineData.at(0);
    QString value = lineData.at(1);

    // Add key and value checks here
    m_configuration->insert(key, QPair<QString, bool>(value, false));

    return true;
}
