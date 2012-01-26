#ifndef CONVERTERMAIN_H
#define CONVERTERMAIN_H

#include <QtCore/qobject.h>
#include <QtCore/qchar.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include "../../ccferror.h"
#include "converterflags.h"
#include "converterfile.h"
#include "converterqrcgenerator.h"

#include <QDebug>

class ConverterCore : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit ConverterCore(const QStringList &args, QObject *parent = 0);
    void convert();

private:
    void setFlags(const QStringList &args);
    void displayHelp();
    void convertDirectory(const QDir &input, const QDir &output);

    bool helpMode;
    ConverterFlags *flags;
};

#endif // CONVERTERMAIN_H
