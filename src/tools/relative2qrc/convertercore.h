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

#ifndef CONVERTERMAIN_H
#define CONVERTERMAIN_H

#include <QtCore/qobject.h>
#include <QtCore/qchar.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include "../../ccferror.h"
#include "converterflags.h"
#include "converterfile.h"
#include "converterqrcgenerator.h"

#include <QDebug>

/*!
  Core class of QRC converter. Manages flags, displays help message,
  initialises the conversion.
  */
class ConverterCore : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit ConverterCore(const QStringList &args, QObject *parent = 0);
    void convert();

private:
    void setFlags(const QStringList &args);
    void displayHelp();
    void convertDirectory(const QDir &input, const QDir &output);

    bool helpMode;
    ConverterFlags *flags;
};

#endif // CONVERTERMAIN_H
