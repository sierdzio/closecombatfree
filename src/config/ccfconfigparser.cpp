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

#include <QByteArray>
#include <QIODevice>
#include <QFile>
#include <QStringList>

#include "ccfconfigparser.h"

/*!
  Constructor, automatically proceeds with parsing the data.

  Parsed config information can be extracted with configuration().

  \sa configuration
  */
CcfConfigParser::CcfConfigParser(const QString &configFilePath, QObject *parent) :
    QObject(parent), CcfError()
{
    mConfiguration = new CcfConfigData();
    mConfigIndexes = new QList<QString>();
    parse(configFilePath);
}

/*!
  Getter for parsed configuration information.

  Returned in a form of QMap<QString, QPair<QString, bool> >, where:
  \li first QString is the key (string before equals sign in config file),
  \li second QString holds the value
  \li bool is false if information was not changed in-game, true if it was
         (for example in preferences menu)

  */
CcfConfigData *CcfConfigParser::configuration()
{
    return mConfiguration;
}

/*!
  Returns an ordered list of configuration options.

  QString holds keys of the configuration file, while the index corresponds
  to position of this entry in that file.
  */
QList<QString> *CcfConfigParser::configIndexes()
{
    return mConfigIndexes;
}

/*!
  Parses the configuration file.
  */
void CcfConfigParser::parse(const QString &configFilePath)
{
    QFile file(configFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        enterErrorState("Could not open the config file.");
        return;
    }

    int i = 0;
    while (!file.atEnd()) {
        if (!readLine(i, file.readLine()))
            return;
        i++;
    }

    file.close();
}

/*!
  Reads a single line from configuration file.
  */
bool CcfConfigParser::readLine(int lineNumber, const QString &lineToParse)
{
    QString line = lineToParse.simplified();

    if (line.isEmpty()) {
        mConfigIndexes->insert(lineNumber, "Empty");
        return true;
    }

    // Detect empty lines and comments
    for (int i = 0; i < line.length(); i++) {
        QChar character = line.at(i);
        if (character.isSpace()) {
            continue; // Could be an empty line or a comment
        } else if (character == QChar('#')) {// || (character == QChar('\n')) || (character == QChar('\0'))) {
            mConfigIndexes->insert(lineNumber, "Comment");
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
    mConfiguration->insert(key, value, false);
    mConfigIndexes->insert(lineNumber, key);

    return true;
}
