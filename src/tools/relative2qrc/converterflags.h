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

/*!
  Class holding flags (console switches) specified by user.
  Shared by all other classes in this project.
  */
class ConverterFlags : public QObject, public CcfError
{
    Q_OBJECT
    Q_FLAGS(Options)

public:
    /*!
      Enum holding state of all application switches.
      */
    enum Option
    {
        InputDirectory   = 0x000001, //!< True if input dir was specified
        OutputDirectory  = 0x000002, //!< True if output dir was specified
        Suffix           = 0x000004, //!< True if suffix was specified
        TempResource     = 0x000008, //!< True if -t switch was set
        Force            = 0x000010, //!< True if -f switch was set
        Skip             = 0x000020, //!< True if --skip switch was set
        NoQrc            = 0x000040, //!< True if --no-qrc switch was set
        NoCore           = 0x000080, //!< True if --no-core switch was set
        NoImg            = 0x000100, //!< True if --no-img switch was set
        NoSkin           = 0x000200  //!< True if --no-skin switch was set
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
