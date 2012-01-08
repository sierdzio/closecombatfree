#include "converterflags.h"

ConverterFlags::ConverterFlags(QObject *parent) :
    QObject(parent), CcfError()
{
}

bool ConverterFlags::setFlagFromString(const QString &flagString)
{
}

ConverterFlags::Options ConverterFlags::flags() const
{
}

QString ConverterFlags::inputDirectory() const
{
}

QString ConverterFlags::outputDirectory() const
{
}

QString ConverterFlags::suffix() const
{
}
