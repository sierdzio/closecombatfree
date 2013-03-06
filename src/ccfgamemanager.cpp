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
#include "qmlBase/ccfqmlbaseunit.h"

/*!
  Initialises the game manager. Pretty important code inside.

  Sets length of tabulation to use, and the name of scenarios folder.
  */
CcfGameManager::CcfGameManager(QObject *parent) :
    QObject(parent), CcfError()
{
    mTab = "    ";

//    QDir scenarioDir("scenarios");
//    m_scenarioList = scenarioDir.entryList(QDir::Files);
    mScenarioList = qmlFileList("scenarios");
    mCampaignList = qmlFileList("campaigns");
}

/*!
  Returns a path for a given scenario from the list.
  */
QString CcfGameManager::scenarioPath(int index) const
{
    return mScenarioList.at(index);
}

/*!
  Getter for scenarios' list.
  */
QStringList CcfGameManager::scenarioList() const
{
    return mScenarioList;
}

/*!
  Returns a path for a given campaign from the list.
  */
QString CcfGameManager::campaignPath(int index) const
{
    return mCampaignList.at(index);
}

/*!
  Getter for campaigns' list.
  */
QStringList CcfGameManager::campaignList() const
{
    return mCampaignList;
}

/*!
  Saves game state to a file.

  For now, it always uses file with name "saves/save1.qml".
  */
void CcfGameManager::saveGame(const QObjectList &unitList,
                              const QString &mapFile,
                              const QString &saveFileName)
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

    // Save units. Hope this will work.
    QString units;
    foreach (QObject *obj, unitList) {
        // In release code, this needs to include order queue,
        // soldier states, damages etc.
        CcfQmlBaseUnit *unit = CcfQmlBaseUnit::ccfUnit(obj);
        units += mTab + unit->getUnitFileName() + " {\n"
                + mTab + mTab + "objectName: \"" + unit->objectName() + "\"\n"
                + mTab + mTab + "x: " + unit->getString("x") + "\n"
                + mTab + mTab + "y: " + unit->getString("y") + "\n"
                + mTab + mTab + "rotation: " + unit->getString("rotation") + "\n"
                + mTab + mTab + "unitSide: \"" + unit->getUnitSide() + "\"\n"
                + mTab + mTab + "state: \"" + unit->getString("state") + "\"\n";
        units += addSavePropertyIfExists(unit, "turretRotation");
        units += addSavePropertyIfExists(unit, "hullColor", true);
        units += mTab + "}\n";
    }
    fileContent.replace("%units%", units);

    QTextStream out(&saveFile);
    out << fileContent;
    saveFile.close();
}

/*!
  Returns a a list of saved games.
  */
QStringList CcfGameManager::savedGamesList() const
{
//    QDir saveDir("saves");
//    return saveDir.entryList(QDir::Files);
    return qmlFileList("saves");
}

/*!
  Returns a list of files in a given directory.

  Useful in getting list of saved games, scenarios, campaigns etc.
  */
QStringList CcfGameManager::qmlFileList(const QString &directoryToSearch) const
{
    QDir saveDir(directoryToSearch);
    return saveDir.entryList(QDir::Files);
}

/*!
  Returns a string representing a QML property - if it existas n \a object.
  */
QString CcfGameManager::addSavePropertyIfExists(const QObject *object,
                                                const QString &propertyName,
                                                bool useQuotes)
{
    QString result;
    QByteArray ba = propertyName.toLocal8Bit();
    if (object->metaObject()->indexOfProperty(ba.data()) != -1) {
        result = mTab + mTab + propertyName + ": ";
        if (useQuotes)
            result += "\"";
        result += object->property(ba.data()).toString();
        if (useQuotes)
            result += "\"";
        result += "\n";
    }
    return result;
}

