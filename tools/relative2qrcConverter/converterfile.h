#ifndef CONVERTERFILE_H
#define CONVERTERFILE_H

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include "../../src/ccferror.h"
#include "converterflags.h"

class ConverterFile : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit ConverterFile(const QString &fileToConvert,
                           const QString &outputFile,
                           QObject *parent = 0);

private:
    ConverterFlags *flags;
};

#endif // CONVERTERFILE_H
