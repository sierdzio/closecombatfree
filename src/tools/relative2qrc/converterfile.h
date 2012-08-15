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

#ifndef CONVERTERFILE_H
#define CONVERTERFILE_H

#include <QtCore/QObject>
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QIODevice>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "../../ccferror.h"
#include "converterflags.h"

//#include <qdebug.h>

/*!
  \ingroup Converter
  @{
 */

/*!
  This class converts a single file, according to CcfFlags object
  and some hard-coded rules. It is also responsible for skipping/ copying
  without change some files.
  */
class ConverterFile : public QObject, public CcfError
{
    Q_OBJECT

public:
    explicit ConverterFile(const QString &fileToConvert,
                           const QString &resultingFile,
                           ConverterFlags *flgs,
                           QObject *parent = 0);

    void convertToQrc();

private:
    int findPath(QString &fileText, int beginIndex);
    int replacePath(QString &fileText, int beginIndex);
    int countJumpsToRoot(const QString &text);
    QString determineQrcPath(const QString &text);

    QString inputFile;
    QString outputFile;
    ConverterFlags *flags;
};

/*! @}*/

#endif // CONVERTERFILE_H
