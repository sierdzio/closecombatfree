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

#include "ccfconfigsaver.h"

/*!
  Pretty much default constructor.
  */
CcfConfigSaver::CcfConfigSaver(QMap<QString, QPair<QString, bool> > *configuration,
                               QList<QString> *configIndexes,
                               const QString &configFilePath,
                               QObject *parent) :
    QObject(parent), m_configuration(configuration),
    m_configIndexes(configIndexes), m_configFilePath(configFilePath)
{
}

/*!
  Updates configuration file based on information about changes made by user.
  */
void CcfConfigSaver::updateConfigFile()
{
    // Check, whether there was any change.
    bool wasChange = false;
    foreach (QString key, m_configuration->keys()) {
        if (m_configuration->value(key).second == true) {
            wasChange = true;
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
