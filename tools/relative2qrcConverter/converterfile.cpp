#include "converterfile.h"

ConverterFile::ConverterFile(const QString &fileToConvert, const QString &outputFile, QObject *parent) :
    QObject(parent), CcfError()
{
}
