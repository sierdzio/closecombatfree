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

#ifndef CCFCONFIGSAVER_H
#define CCFCONFIGSAVER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>

#include "../ccferror.h"
#include "ccfconfigdata.h"

/*!
  \ingroup CloseCombatFree
  @{
 */

/*!
  Saves configuration back to a file.
  */
class CcfConfigSaver : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit CcfConfigSaver(CcfConfigData *configuration,
                            QList<QString> *configIndexes,
                            const QString &configFilePath,
                            QObject *parent = 0);
    void updateConfigFile();

private:
    CcfConfigData *m_configuration;
    QList<QString> *m_configIndexes;
    QString m_configFilePath;
};

/*! @}*/

#endif // CCFCONFIGSAVER_H
