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

#ifndef CCFCONFIGPARSER_H
#define CCFCONFIGPARSER_H

#include <QObject>
#include <QString>
#include <QList>

#include "../ccferror.h"
#include "ccfconfigdata.h"

/*!
  \ingroup CloseCombatFree
  @{
 */

/*!
  Class for parsing configuration file.

  Parsed options are stored in QMap<QString, QPair<QString, bool> >, where
  first QString is the option name, the second QString
  (accessible through .first) stores value, and the bool specifies, whether
  the value was changed in-game (true if was, false by default).

  Additional QList<QString> stores option names in ordered manner - this way
  file saving can be optimised.
  */
class CcfConfigParser : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit CcfConfigParser(const QString &configFilePath, QObject *parent = 0);
    CcfConfigData *configuration();
    QList<QString> *configIndexes();

private:
    void parse(const QString &configFilePath);
    bool readLine(int lineNumber, const QString &line);

    CcfConfigData *mConfiguration;
    QList<QString> *mConfigIndexes;
};

/*! @}*/

#endif // CCFCONFIGPARSER_H
