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

#include <QFile>

#include "ccfglobal.h"

/*!
  A simple constructor. Initialises parent QObject and CcfError.
  */
CcfGlobal::CcfGlobal(QObject *parent) :
    QObject(parent), CcfError()
{
}

/*!
  Emits the signal that triggers disabling the use of qrc.

  This will probably get optimised away in the future.
  */
void CcfGlobal::disableQrcUse(QObject *object)
{
    emit disableQrc(object);
}

/*!
  Returns contents of a file (specified in \a filePath) as a QString.

  PLease note that paths are always resolved from the directory where the BINARY
  is located!
  */
QString CcfGlobal::getFileContents(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        enterErrorState("Could not open file to read: " + file.fileName());
        qWarning(qPrintable(("Could not open file to read: %1")), qPrintable(file.fileName()));
        return QString();
    }

    QString result = file.readAll();
    file.close();
    return result;
}
