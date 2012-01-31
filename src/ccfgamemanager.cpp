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

#include "ccfgamemanager.h"

CcfGameManager::CcfGameManager(QObject *parent) :
    QObject(parent), CcfError()
{
    tab = "    ";

    QDir scenarioDir("scenarios");
    m_scenariosList = scenarioDir.entryList(QDir::Files);
}

QStringList CcfGameManager::scenariosList()
{
    return m_scenariosList;
}

QString CcfGameManager::scenarioPath(int index)
{
    return m_scenariosList.at(index);
}

void CcfGameManager::saveGame(const QDeclarativeListReference &unitsList, const QString &mapFile, const QString &saveFileName)
{
    // As a first attempt, I will generate the whole file myself.
    // A better approach for the future might be to copy and modify
    // a real scenario file, OR create a QML element like ScenarioLoader
    // which would have "map", "units" properties.

    // Init. Read template.
    QFile templateFile("src/config/saveFileTemplate.txt");
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
                + tab + tab + "rotation: " + unit->property("rotation").toString() + "\n"
                + tab + tab + "unitSide: \"" + unit->property("unitSide").toString() + "\"\n";
        units += addSavePropertyIfExists(unit, "turretRotation");
        units += addSavePropertyIfExists(unit, "hullColor", true);
        units += tab + "}\n";
    }
    fileContent.replace("%units%", units);

    QTextStream out(&saveFile);
    out << fileContent;
    saveFile.close();
}

QStringList CcfGameManager::savedGamesList()
{
    QDir saveDir("saves");
    return saveDir.entryList(QDir::Files);
}

QString CcfGameManager::addSavePropertyIfExists(const QObject *object,
                                                const QString &propertyName,
                                                bool useQuotes)
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
