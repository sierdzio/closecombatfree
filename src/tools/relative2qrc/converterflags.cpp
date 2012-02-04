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

#include "converterflags.h"

/*!
  \ingroup Converter
  @{
 */

/*!
  Pretty much the default constructor.
  */
ConverterFlags::ConverterFlags(QObject *parent) :
    QObject(parent), CcfError()
{
//    m_flags &= ConverterFlags::Suffix;
//    m_suffix = "_old";
}

/*!
  Parses a list of arguments (\a appArguments).

  Based on information in the parsed list, it populates the flag parameter,
  as well as some other ones (input path, suffix etc.).
  */
bool ConverterFlags::setFlags(const QStringList &appArguments)
{
    for (int i = 0; i < appArguments.length(); ++i) {
        if (isErrorState()) {
            return false;
        }

        QString current = appArguments.at(i);

        // This is a flag.
        if (current.startsWith(QChar('-')) || current.startsWith(QLatin1String("--"))) {
            if (current == "-o") {
                m_flags |= ConverterFlags::OutputDirectory;
                m_outputDirectory = checkPath(appArguments.at(++i));
            } else if (current == "-i") {
                m_flags |= ConverterFlags::InputDirectory;
                m_inputDirectory = checkPath(appArguments.at(++i));
            } else if ((current == "-s") || (current == "--suffix")) {
                m_flags |= ConverterFlags::Suffix;
                m_suffix = appArguments.at(++i);
            } else if (current == "--skip") {
                m_flags |= ConverterFlags::Skip;
                m_skip = parseSkipCommand(appArguments.at(++i));
            } else if ((current == "-f") || (current == "--force")) {
                m_flags |= ConverterFlags::Force;
            } else if ((current == "-t") || (current == "--temp-resource")) {
                m_flags |= ConverterFlags::TempResource;
            } else if (current == "--no-qrc") {
                m_flags |= ConverterFlags::NoQrc;
            } else if (current == "--no-core") {
                m_flags |= ConverterFlags::NoCore;
            } else if (current == "--no-img") {
                m_flags |= ConverterFlags::NoImg;
            } else if (current == "--no-skin") {
                m_flags |= ConverterFlags::NoSkin;
            } else {
                enterErrorState(current + " is not a valid flag.");
            }
        } else {
            enterErrorState(current + " is not a valid flag.");
        }
    }

    return !isErrorState();
}

/*!
  Returns the parsed flags as a QFlag (enum).
  */
ConverterFlags::Options ConverterFlags::flags() const
{
    return m_flags;
}

/*!
  Returns the input directory, or empty string if it was not specified.

  You can check whether it was specified by checking flags
  (for example: if (ConverterFlags::flags() & ConverterFlags::InputDirectory)
  ...).
  */
QString ConverterFlags::inputDirectory() const
{
    return m_inputDirectory;
}

/*!
  Returns the output directory, or empty string if it was not specified.

  You can check whether it was specified by checking flags
  (for example: if (ConverterFlags::flags() & ConverterFlags::OutputDirectory)
  ...).
  */
QString ConverterFlags::outputDirectory() const
{
    return m_outputDirectory;
}

/*!
  Returns the output directory, or empty string if it was not specified.

  You can check whether it was specified by checking flags
  (for example: if (ConverterFlags::flags() & ConverterFlags::Suffix)
  ...).
  */
QString ConverterFlags::suffix() const
{
    return m_suffix;
}

/*!
  Returns a list of filter strings specifying files to skip.
  */
QStringList ConverterFlags::skip() const
{
    return m_skip;
}

QString ConverterFlags::checkPath(const QString &pathToCheck)
{
    // TODO: Need to add true checking here!
    return QDir::fromNativeSeparators(pathToCheck);
}

QStringList ConverterFlags::parseSkipCommand(const QString &commandText)
{
    // TODO: check for wrong entries!
    return commandText.split(QChar(','));
}

/*! @}*/
