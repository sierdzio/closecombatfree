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

#include "convertercore.h"
#include "iostream"

/*!
  \ingroup Converter
  @{
 */

ConverterCore::ConverterCore(const QStringList &args, QObject *parent) :
    QObject(parent), CcfError()
{
    helpMode = false;
    if (args.contains("-h") || args.contains("--help")) {
        displayHelp();
        helpMode = true;
    } else {
        flags = new ConverterFlags(this);
        setFlags(args);
    }
}

void ConverterCore::setFlags(const QStringList &args)
{
    bool result = false;

    // If first arg is not a flag - it's probably the app name.
    // In such a case, that arg will be discarded.
    if (!args.at(0).startsWith(QChar('-')) && !args.at(0).startsWith(QLatin1String("--"))) {
        result = flags->setFlags(args.mid(1));
    } else {
        result = flags->setFlags(args);
    }

    if (!result) {
        enterErrorState(flags->errorMessage());
    }
}

void ConverterCore::convert()
{
    if (helpMode)
        return;

    if ((flags->inputDirectory() == flags->outputDirectory())
            && !(flags->flags() & ConverterFlags::Force)
            && !(flags->flags() & ConverterFlags::Suffix)) {
        enterErrorState("Will not overwrite without --force or a given --suffix.");
        return;
    }

    QString input;
    QString output;

    if (flags->inputDirectory() == "") {
        input = ".";
    } else {
        input = flags->inputDirectory();
    }

    if (flags->outputDirectory() == "") {
        output = ".";
    } else {
        output = flags->outputDirectory();
    }

    QDir inputDir(input);
    QDir outputDir(output);

    convertDirectory(inputDir, outputDir);

    ConverterQrcGenerator qrcGenerator(flags, this);
    qrcGenerator.createQrcFiles();
    if (qrcGenerator.isErrorState()) {
        enterErrorState(qrcGenerator.errorMessage());
    }
}

void ConverterCore::convertDirectory(const QDir &input, const QDir &output)
{
    QDir tempInput = input;
    tempInput.makeAbsolute();
    if (!tempInput.exists()) {
        qFatal("Error! Input: %s does not exist!", tempInput.path().toLocal8Bit().data());
    }

    QStringList dirs(tempInput.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    QStringList files(tempInput.entryList(QDir::Files));

    QDir tempOutput = output;
    tempOutput.makeAbsolute();
    if (!tempOutput.exists()) {
        if (!tempOutput.mkpath(output.path()))
            qFatal("Creating directory failed! %s", tempOutput.path().toLocal8Bit().data());
    }

    foreach (const QString &file, files) {
        ConverterFile converter(tempInput.path() + "/" + file, output.path() + "/" + file, flags);
        converter.convertToQrc();

        if (converter.isErrorState()) {
            enterErrorState(converter.errorMessage());
            return;
        }
    }

    foreach (const QString &dir, dirs) {
        if ((dir.at(0) == QChar('.'))
                || (dir.right(5) == "build")
                || (dir.right(flags->outputDirectory().length()) == flags->outputDirectory())) {
            // Don't touch hidden directiories
            // Don't touch build directory
            // Don't touch output directory
            continue;
        }

        QString outputPath = tempOutput.path() + "/" + dir;
        QDir outputDir(outputPath);
        QDir inputDir(tempInput.path() + "/" + dir);
        convertDirectory(inputDir, outputDir);
    }
}

void ConverterCore::displayHelp()
{
//    std::cout << "This help message is still a WIP." << std::endl;
    QFile help(":/converter/helpMessage.txt");

    if (!help.open(QIODevice::ReadOnly | QIODevice::Text)) {
        enterErrorState("Could not open the help message file! Please contact game developers.");
        return;
    }

    QByteArray helpData = help.readAll();
    std::cout << helpData.data() << std::endl;
}

/*! @}*/
