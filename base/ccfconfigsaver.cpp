#include "ccfconfigsaver.h"
#include <QDebug>

CcfConfigSaver::CcfConfigSaver(QMap<QString, QPair<QString, bool> > *configuration,
                               QList<QString> *configIndexes,
                               QString configFilePath,
                               QObject *parent) :
    QObject(parent), m_configuration(configuration),
    m_configIndexes(configIndexes), m_configFilePath(configFilePath)
{
}

void CcfConfigSaver::updateConfigFile()
{
    // Check, whether there was any change.
    bool wasChange = false;
    foreach (QString key, m_configuration->keys()) {
        if (m_configuration->value(key).second == true) {
            wasChange = true;
            qDebug() << "Value change for: " << key << " = " << m_configuration->value(key).first
                     << ", " << m_configuration->value(key).second;
            break;
        }
    }

    // If no config change was detected, skip writing.
    if (wasChange == false) {
        return;
    }

    // Else, look through the file for the changed lines, and replace them.
    // Old config is renamed to "config_old", new one into "config".
    QFile::remove(m_configFilePath + "_old");
    QFile::rename(m_configFilePath, m_configFilePath + "_old");

    QFile file(m_configFilePath + "_old");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        enterErrorState("Could not open the config file.");
        return;
    }

    QFile newFile(m_configFilePath);
    if (!newFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        enterErrorState("Could not open the config file.");
        return;
    }

    int i = 0;
    while (!file.atEnd()) {
        QString replacement;
        QString line = file.readLine();
        QString option = m_configIndexes->at(i);
        // Skip comments
        if ((option == "Empty")
                || (option == "Comment")) {
            newFile.write(line.toUtf8());
            i++;
            continue;
        }

        if (m_configuration->contains(option)) {
            if (m_configuration->value(option).second == true) {
                if (line.contains(" = ")) {
                    replacement = option + " = " + m_configuration->value(option).first;
                } else if (line.contains("=")) {
                    replacement = option + "=" + m_configuration->value(option).first;
                }

                // Replace the needed fragment of code:
                newFile.write(replacement.toUtf8() + "\n");
            } else {
                newFile.write(line.toUtf8());
            }
        }

        i++;
    }

    file.close();
    newFile.close();
}
