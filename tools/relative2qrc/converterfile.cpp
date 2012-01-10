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

    qDebug() << inputFile << outputFile;

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
    int endIndex = fileText.indexOf("\"", beginIndex);// + 1;

    fileText.replace(beginIndex, endIndex - beginIndex, "Temp test text");

    return -1;
}
