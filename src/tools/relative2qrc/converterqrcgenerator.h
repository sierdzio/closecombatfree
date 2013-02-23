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

#ifndef CONVERTERQRCGENERATOR_H
#define CONVERTERQRCGENERATOR_H

#include <QObject>
#include <QChar>
#include <QString>
#include <QStringList>
#include <QIODevice>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include "../../ccferror.h"
#include "converterflags.h"

//#include <QDebug>

/*!
  \ingroup Converter
  @{
 */

/*!
  QRC generating class. Creates fragmented resources based on a set of
  hard-coded rules. Some rules can be specified by flags, too.
  */
class ConverterQrcGenerator : public QObject, public CcfError
{
    Q_OBJECT

public:
    explicit ConverterQrcGenerator(ConverterFlags *flgs, QObject *parent = 0);
    void createQrcFiles();

private:
    void createCoreResource();
    void createImgResource();
    void createSkinResource();
    void createTempResource();
    void createResource(const QString &file, const QString &body);
    QString addAllFiles(const QString &directory, bool useAlias = true);
    QString addSingleFile(const QString &alias, const QString &file, bool useAlias = true);
    QString recursivelyAddAllFiles(const QString &directory);

    ConverterFlags *flags;
    QString inputPath;
    QString outputPath;
    QString tab;
};

/*! @}*/

#endif // CONVERTERQRCGENERATOR_H
