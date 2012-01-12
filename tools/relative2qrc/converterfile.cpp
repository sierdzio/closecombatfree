#include "converterfile.h"

ConverterFile::ConverterFile(const QString &fileToConvert, const QString &resultingFile,
                             ConverterFlags *flgs, QObject *parent) :
    QObject(parent), CcfError(), flags(flgs), inputFile(fileToConvert), outputFile(resultingFile)
{
}

void ConverterFile::convertToQrc()
{
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
    if (!(flags->flags() & ConverterFlags::Force)) {
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
    QString newPath("qrc:/" + determineQrcPath(oldPath) );

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

    if ((dirs.at(0) == "qml")
            && (dirs.length() > 1) // Prevents segfault in next line
            && (dirs.at(1) == "gui")) {
        result = "skin";
    } else if (dirs.at(0) == "qml") {
        result = "core";
    } else {
        result = dirs.at(0);
    }

    for (int i = 1; i < dirs.length(); ++i) {
        result += "/" + dirs.at(i);
    }

    return result;
}
