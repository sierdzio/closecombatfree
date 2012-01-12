#include "converterqrcgenerator.h"

ConverterQrcGenerator::ConverterQrcGenerator(ConverterFlags *flgs, QObject *parent) :
    QObject(parent), CcfError(), flags(flgs)
{
    tab = "    ";

    if (flags->inputDirectory() == "") {
        inputPath = ".";
    } else {
        inputPath = flags->inputDirectory();
    }

    if (flags->outputDirectory() == "") {
        outputPath = ".";
    } else {
        outputPath = flags->outputDirectory();
    }
}

void ConverterQrcGenerator::createQrcFiles()
{
    if (flags->flags() & ConverterFlags::NoQrc) {
        return;
    }

    if (!(flags->flags() & ConverterFlags::NoCore)) {
        createCoreResource();
    }

    if (!(flags->flags() & ConverterFlags::NoImg)) {
        createImgResource();
    }

    if (!(flags->flags() & ConverterFlags::NoSkin)) {
        createSkinResource();
    }

    if (flags->flags() & ConverterFlags::TempResource) {
        createTempResource();
    }
}

void ConverterQrcGenerator::createCoreResource()
{
    QString body;

    QDir tempInput(inputPath);
    if (!tempInput.exists()) {
        qFatal("Error! Input: %s does not exist!", tempInput.path().toLocal8Bit().data());
    }

    body += addAllFiles(inputPath + "/qml");
    tempInput.setPath(inputPath + "/qml");
    foreach (const QString &dir, tempInput.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (dir == "gui") {
            continue;
        }
        body += recursivelyAddAllFiles(inputPath + "/qml/" + dir);
    }

    body += addSingleFile("config/saveFileTemplate.txt", "src/config/saveFileTemplate.txt", true);

    body.replace("\"qml/", "\"");

    createResource("core", body);
}

void ConverterQrcGenerator::createImgResource()
{
    QString body;

    QDir tempInput(inputPath);
    if (!tempInput.exists()) {
        qFatal("Error! Input: %s does not exist!", tempInput.path().toLocal8Bit().data());
    }

    body = recursivelyAddAllFiles(inputPath + "/img");
    body.replace("\"img/", "\"");

    createResource("img", body);
}

void ConverterQrcGenerator::createSkinResource()
{
    QString body;

    QDir tempInput(inputPath);
    if (!tempInput.exists()) {
        qFatal("Error! Input: %s does not exist!", tempInput.path().toLocal8Bit().data());
    }

    body = recursivelyAddAllFiles(inputPath + "/qml/gui");
    body.replace("\"qml/gui/", "\"");

    createResource("skin", body);
}

void ConverterQrcGenerator::createTempResource()
{
    QString body;

    QDir tempInput(inputPath);
    if (!tempInput.exists()) {
        qFatal("Error! Input: %s does not exist!", tempInput.path().toLocal8Bit().data());
    }

    body = recursivelyAddAllFiles(inputPath + "/scenarios");
    body += recursivelyAddAllFiles(inputPath + "/maps");
    body += recursivelyAddAllFiles(inputPath + "/units");
    body += addSingleFile("", "config", false);
    body += addSingleFile("", "config_old", false);

    createResource("temp", body);
}

void ConverterQrcGenerator::createResource(const QString &file, const QString &body)
{
    QFile output(outputPath + "/src/" + file + "Resource.qrc");

    if (output.exists()) {
        output.remove();
    }

    if (!output.open(QIODevice::WriteOnly | QIODevice::Text)) {
        enterErrorState("Could not open file to save data: " + output.fileName());
        return;
    }

    QTextStream out(&output);
    out << "<RCC>" << endl;
    out << tab << "<qresource prefix=\"/" + ((file == "temp")? "" : file) + "\">" << endl;
    out << body; // Other methods are responsible for formating here.
    out << tab << "</qresource>" << endl;
    out << "</RCC>" << endl;
    output.close();
}

QString ConverterQrcGenerator::addAllFiles(const QString &directory, bool useAlias)
{
    QString result;
    QStringList files = QDir(directory).entryList(QDir::Files);
    foreach (const QString &file, files) {
        QString path = directory.mid(2);

        if (directory == ".") {
            path += file;
        } else {
            path += "/" + file;
        }

        result += addSingleFile(path, path, useAlias);
    }
    return result;
}

QString ConverterQrcGenerator::addSingleFile(const QString &alias, const QString &file, bool useAlias)
{
    QString result(tab + tab);

    QString openingTag("<file");
    if (useAlias) {
        openingTag += " alias=\"" + alias + "\"";
    }
    openingTag += ">";

    result += openingTag + "../" + file + "</file>\n";

    return result;
}

QString ConverterQrcGenerator::recursivelyAddAllFiles(const QString &directory)
{
    QString result = addAllFiles(directory);
    QStringList dirs = QDir(directory).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &dir, dirs) {
         result += recursivelyAddAllFiles(directory + "/" + dir);
    }
    return result;
}
