#ifndef CONVERTERFILE_H
#define CONVERTERFILE_H

#include <QtCore/qobject.h>
#include <QtCore/qchar.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qfile.h>
#include <QtCore/qtextstream.h>
#include "../../src/ccferror.h"
#include "converterflags.h"

#include <qdebug.h>

class ConverterFile : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit ConverterFile(const QString &fileToConvert,
                           const QString &resultingFile,
                           ConverterFlags *flgs,
                           QObject *parent = 0);

    void convertToQrc();

private:
    int findPath(QString &fileText, int beginIndex);
    int replacePath(QString &fileText, int beginIndex);

    QString inputFile;
    QString outputFile;
    ConverterFlags *flags;
};

#endif // CONVERTERFILE_H
