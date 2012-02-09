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

#ifndef CCFGLOBAL_H
#define CCFGLOBAL_H

#include <QObject>
#include <QString>

#include "ccferror.h"

/*!
  \ingroup CloseCombatFree
  @{
 */

/*!
  Class used as "Global" object in QML.

  Has some globally needed methods, like disabling QRC on a QObject,
  sending statusMessages, etc.
  */
class CcfGlobal : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit CcfGlobal(QObject *parent = 0);
    Q_INVOKABLE void disableQrcUse(QObject *object);

public slots:
    void statusMsg(const QString &message);
    void statusMessage(const QString &message);

signals:
    /*!
      Emited when a new status message is requested.
      */
    void newStatusMessage(const QString &message,
                          QObject *sender);

    /*!
      Emited when disabling QRC is requested.
      */
    void disableQrc(QObject *object);
};

/*! @}*/

#endif // CCFGLOBAL_H
