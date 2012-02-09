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

#include "converterfile.h"

/*!
  A simple constructor. Not much going on inside.
  */
ConverterFile::ConverterFile(const QString &fileToConvert, const QString &resultingFile,
                             ConverterFlags *flgs, QObject *parent) :
    QObject(parent), CcfError(), flags(flgs), inputFile(fileToConvert), outputFile(resultingFile)
{
//    qDebug() << inputFile << "=>" << outputFile;
}

/*!
  Converts a single file's paths into QRC paths.

  A lot of things happening here. This method handles skipping of some files,
  and has some hard-coded rules for certain files (like C++ ones, where
  paths are a bit different).
  */
void ConverterFile::convertToQrc()
{
    // Skip .pro.user files
    if ((inputFile.right(9) == ".pro.user")
            || (inputFile == "Makefile")
            || (inputFile.at(inputFile.length() - 1) == QChar('~'))) {
        return;
    }

    int lastDotIndex = inputFile.lastIndexOf(QChar('.'));
    QString extension = inputFile.right(lastDotIndex + 1);

    if (extension == "xcf") {
        return;
    }

    QFile input(inputFile);
    QFile output(outputFile);

    // Handle skipping
    if (flags->flags() & ConverterFlags::Skip) {
        foreach (const QString &s, flags->skip()) {
            if (input.fileName() == s) {
                return;
            } else if (s.at(0) == QChar('*')) {
                QString temp = s.mid(1);
                if (input.fileName().right(temp.length()) == temp) {
                    return;
                }
            }
        }
    }

    // If we are not allowed to overwrite, but source exists.
    if ((output.exists())
            && !(flags->flags() & ConverterFlags::Suffix)
            && !(flags->flags() & ConverterFlags::Force)) {
        enterErrorState("Destination file already exists. Will not overwrite.\n"
                        "If you want to force overwrite, use -f (--force).\n"
                        "If you prefer to backing-up, use -s (--suffix).");
        return;
    }

    // All files that do not require conversion.
    // TODO: handle C++ files.
    QString tempQml = inputFile.right(4);
    QString tempJs = inputFile.right(3);
    if ((tempQml != ".qml")
            && (tempJs != ".js")
            && (inputFile.right(8) != "main.cpp")
            && (inputFile.right(7) != "src.pro")
            && (inputFile.right(13) != "ccfconfig.cpp")) {
        if (flags->flags() & ConverterFlags::Force) {
            output.remove();
        }

        if (input.copy(outputFile + flags->suffix())) {
            return;
        } else {
            enterErrorState("Could not copy file: " + input.fileName()
                            + ", to: " + (outputFile + flags->suffix()));
            return;
        }
    }

    // QML and JS file handling.
    // TODO: (optional) move to a separate method.
    if (!input.open(QIODevice::ReadOnly | QIODevice::Text)) {
        enterErrorState("Could not open file: " + input.fileName());
        return;
    }

    if (!output.open(QIODevice::WriteOnly | QIODevice::Text)) {
        enterErrorState("Could not open file to save data: " + output.fileName());
        return;
    }

    // First try: with readAll().
    // If it's bad, we can move to reading line by line.
    int beginIndex = 0;
    QString inputData = input.readAll();

    // main.cpp handling
    if (inputFile.right(8) == "main.cpp") {
        inputData.replace("qml/main.qml", "qrc:/core/main.qml");
    } else if (inputFile.right(7) == "src.pro") {
        inputData.replace("#RESOURCES", "RESOURCES");
        inputData.replace("#    ", "    ");
    } else if (inputFile.right(13) == "ccfconfig.cpp") {
        inputData.replace("QDir scenarioDir(\"scenarios\");", "QDir scenarioDir(\":/scenarios\");");
        inputData.replace("QDir saveDir(\"saves\");", "QDir saveDir(\":/saves\");");
    } else {
        if (inputFile.right(16) == "ScenarioMenu.qml") {
            inputData.replace("\"scenarios/\" + scenarioPath;", "\"qrc:/scenarios/\" + scenarioPath;");
            inputData.replace("\"saves/\" + path;", "\"qrc:/saves/\" + path;");
        }
        // All remaining files
        forever {
            beginIndex = findPath(inputData, beginIndex);
            if (beginIndex == -1) {
                break;
            } else {
                beginIndex = replacePath(inputData, beginIndex);
                ++beginIndex;
            }
        }
    }

    input.close();
    QTextStream out(&output);
    out << inputData;
    output.close();
}

/*!
  Locates next path in data string.
  */
int ConverterFile::findPath(QString &fileText, int beginIndex)
{
    return fileText.indexOf("\"../", beginIndex);
}

/*!
  Replaces a relative path with QRC path.
  */
int ConverterFile::replacePath(QString &fileText, int beginIndex)
{
    ++beginIndex; // To move past the opening quotation mark.
    int endIndex = fileText.indexOf("\"", beginIndex);

    QString oldPath = fileText.mid(beginIndex, endIndex - beginIndex);
    QString newPath("qrc:/" + determineQrcPath(oldPath) );

    fileText.replace(beginIndex, endIndex - beginIndex, newPath);

    return -1;
}

/*!
  Counts directory jumps that need to be made to get to root dir
  for a given path.
  */
int ConverterFile::countJumpsToRoot(const QString &text)
{
    int result = 0;
    int beginIndex = 0;

    forever {
        beginIndex = text.indexOf("../", beginIndex);
        if (beginIndex == -1) {
            break;
        } else {
            ++beginIndex;
            ++result;
        }
    }

    return result;
}

/*!
  Chooses which QRC prefix to use.

  Options:
   - gui
   - skin
   - core
   - img
  */
QString ConverterFile::determineQrcPath(const QString &text)
{
    QString result;
    QStringList dirs;
    int beginIndex = text.lastIndexOf("../") + 2;
    int endIndex = text.length();

    if (beginIndex == endIndex) { // There is only one dir after path jumps
        ++beginIndex;
        ++endIndex;
        dirs.append(text.mid(endIndex));
    } else { // There are many dirs after path jumps
        ++beginIndex;
        QString subpath(text.mid(beginIndex, endIndex - beginIndex));
        dirs = subpath.split(QChar('/'));
    }

    int i = 0; // Starting point
    if ((dirs.at(0) == "qml")
            && (dirs.length() > 1) // Prevents segfault in next line
            && (dirs.at(1) == "gui")) {
        result = "skin";
        i = 2;
    } else if (dirs.at(0) == "qml") {
        result = "core";
        i = 1;
    } else {
        result = dirs.at(0);
        i = 1;
    }

    for (; i < dirs.length(); ++i) {
        result += "/" + dirs.at(i);
    }

    return result;
}
