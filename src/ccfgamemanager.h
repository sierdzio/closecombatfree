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

#ifndef CCFGAMEMANAGER_H
#define CCFGAMEMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QQmlListReference>

#include "ccferror.h"

/*!
  \defgroup CloseCombatFree Game Code
  @{
 */

/*!
  Class used in QML code as "GameManager" global object.

  Can return all sorts of data about available scenarios
  and saved games.

  Is also used to save game state to a file.
  */
class CcfGameManager : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit CcfGameManager(QObject *parent = 0);    
    Q_INVOKABLE QString scenarioPath(int index);
    Q_INVOKABLE void saveGame(const QQmlListReference &unitsList,
                              const QString &mapFile,
                              const QString &saveFileName = "saves/save1.qml");
    Q_INVOKABLE QStringList qmlFileList(const QString &directoryToSearch);
    Q_INVOKABLE QStringList savedGamesList();
    Q_INVOKABLE QStringList scenariosList();
    
private:
    QString addSavePropertyIfExists(const QObject *object,
                                    const QString &propertyName,
                                    bool useQuotes = false);

    QStringList mScenariosList;
    QString mTab;
};

/*! @}*/

#endif // CCFGAMEMANAGER_H
