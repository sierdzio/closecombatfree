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

    // All files that do not require conversion.
    // TODO: handle C++ files.
    QString tempExt = inputFile.right(4);
    if ((tempExt != ".qml") && (tempExt != ".js")) {
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

    if (!output.open(QIODevice::ReadOnly | QIODevice::Text)) {
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
}

int ConverterFile::findPath(QString &fileText, int beginIndex)
{
    int endIndex = 0;

    beginIndex = fileText.indexOf("\"", beginIndex);
    endIndex = fileText.indexOf("\"", beginIndex + 1) + 1;

    if ((fileText.mid(endIndex - 4, 4) == ".qml") || (fileText.mid(endIndex - 3, 3) == ".js")) {
        // This indeed is a path.
        return beginIndex;
    }

    return -1;
}

int ConverterFile::replacePath(QString &fileText, int beginIndex)
{
    int endIndex = fileText.indexOf("\"", beginIndex + 1) + 1;

    fileText.replace(beginIndex, endIndex - beginIndex, "Temp test text");

    return -1;
}
