#ifndef CONVERTERQRCGENERATOR_H
#define CONVERTERQRCGENERATOR_H

#include <QtCore/qobject.h>
#include <QtCore/qchar.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qtextstream.h>
#include "../../ccferror.h"
#include "converterflags.h"

#include <qdebug.h>

class ConverterQrcGenerator : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit ConverterQrcGenerator(ConverterFlags *flgs, QObject *parent = 0);
    void createQrcFiles();

private:
    void createCoreResource();
    void createImgResource();
    void createSkinResource();
    void createTempResource();
    void createResource(const QString &file, const QString &body);
    QString addAllFiles(const QString &directory, bool useAlias = true);
    QString addSingleFile(const QString &alias, const QString &file, bool useAlias = true);
    QString recursivelyAddAllFiles(const QString &directory);

    ConverterFlags *flags;
    QString inputPath;
    QString outputPath;
    QString tab;
};

#endif // CONVERTERQRCGENERATOR_H
