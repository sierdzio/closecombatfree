#include "converterfile.h"

ConverterFile::ConverterFile(const QString &fileToConvert, const QString &resultingFile,
                             ConverterFlags *flgs, QObject *parent) :
    QObject(parent), CcfError(), flags(flgs), inputFile(fileToConvert), outputFile(resultingFile)
{
}

void ConverterFile::convertToQrc()
{
    QFile input(inputFile);
    QFile output(outputFile + flags->suffix());

//    qDebug() << inputFile << outputFile;

    // All files that do not require conversion.
    // TODO: handle C++ files.
    QString tempQml = inputFile.right(4);
    QString tempJs = inputFile.right(3);
    if ((tempQml != ".qml") && (tempJs != ".js")) {
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

    forever {
        beginIndex = findPath(inputData, beginIndex);
        if (beginIndex == -1) {
            break;
        } else {
            beginIndex = replacePath(inputData, beginIndex);
            ++beginIndex;
        }
    }

    input.close();
    QTextStream out(&output);
    out << inputData;
    output.close();
}

int ConverterFile::findPath(QString &fileText, int beginIndex)
{
    return fileText.indexOf("\"../", beginIndex);
}

int ConverterFile::replacePath(QString &fileText, int beginIndex)
{
    ++beginIndex; // To move past the opening quotation mark.
    int endIndex = fileText.indexOf("\"", beginIndex);

    QString oldPath = fileText.mid(beginIndex, endIndex - beginIndex);
//    int jumpsUp = countJumpsToRoot(oldPath);

    QString file(oldPath.mid(oldPath.lastIndexOf('/') + 1));
    if (!file.contains(QChar('.'))) {
        file.clear();
    }

    QString newPath("qrc:/"
                    + determineQrcPath(oldPath)
                    + "/"
                    + file);

    fileText.replace(beginIndex, endIndex - beginIndex, newPath);

    return -1;
}

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

QString ConverterFile::determineQrcPath(const QString &text)
{
    QString result;
    QStringList dirs;
    int beginIndex = text.lastIndexOf("../") + 2;
    int endIndex = text.lastIndexOf("/");

    if (beginIndex == endIndex) { // There is only one dir after path jumps
        ++beginIndex;
        ++endIndex;
        dirs.append(text.mid(endIndex));
    } else { // There are many dirs after path jumps
        ++beginIndex;
        QString subpath(text.mid(beginIndex, endIndex - beginIndex));
        dirs = subpath.split(QChar('/'));
    }

//    qDebug() << text << "|" << dirs.length() << "|" << dirs;

    if ((dirs.at(0) == "qml")
            && (dirs.length() > 1) // Prevents segfault in next line
            && (dirs.at(1) == "gui")) {
        result = "skin";
    } else if (dirs.at(0) == "qml") {
        result = "core";
    } else {
//        // Absolute fallback. Dangerous!
//        result = text.mid(text.lastIndexOf("../") + 3);

        result = text.mid(beginIndex, endIndex - beginIndex);
    }

    return result;
}
