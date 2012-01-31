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

#ifndef CONVERTERFLAGS_H
#define CONVERTERFLAGS_H

#include <QtCore/qobject.h>
#include <QtCore/qchar.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qdir.h>
#include <QtCore/QFlag>
#include <QtCore/QFlags>
#include <qdebug.h>
#include "../../ccferror.h"

class ConverterFlags : public QObject, public CcfError
{
    Q_OBJECT
    Q_FLAGS(Options)

public:
    enum Option
    {
        InputDirectory   = 0x000001,
        OutputDirectory  = 0x000002,
        Suffix           = 0x000004,
        TempResource     = 0x000008,
        Force            = 0x000010,
        Skip             = 0x000020,
        NoQrc            = 0x000040,
        NoCore           = 0x000080,
        NoImg            = 0x000100,
        NoSkin           = 0x000200
    };
    Q_DECLARE_FLAGS(Options, Option)

    explicit ConverterFlags(QObject *parent = 0);

    // Setters:
    bool setFlags(const QStringList &appArguments);

    // Getters:
    Options flags() const;
    QString inputDirectory() const;
    QString outputDirectory() const;
    QString suffix() const;
    QStringList skip() const;

private:
    QString checkPath(const QString &pathToCheck);
    QStringList parseSkipCommand(const QString &commandText);

    Options m_flags;
    QString m_inputDirectory;
    QString m_outputDirectory;
    QString m_suffix;
    QStringList m_skip;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ConverterFlags::Options)

#endif // CONVERTERFLAGS_H
