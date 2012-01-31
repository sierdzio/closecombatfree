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
#include <QDeclarativeListReference>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "ccferror.h"

class CcfGameManager : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit CcfGameManager(QObject *parent = 0);
    Q_INVOKABLE QStringList scenariosList();
    Q_INVOKABLE QString scenarioPath(int index);
    Q_INVOKABLE void saveGame(const QDeclarativeListReference &unitsList,
                              const QString &mapFile,
                              const QString &saveFileName = "saves/save1.qml");
    Q_INVOKABLE QStringList savedGamesList();
    
private:
    QString addSavePropertyIfExists(const QObject *object,
                                    const QString &propertyName,
                                    bool useQuotes = false);

    QStringList m_scenariosList;
    QString tab;
};

#endif // CCFGAMEMANAGER_H
