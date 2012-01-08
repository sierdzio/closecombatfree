#ifndef CONVERTERMAIN_H
#define CONVERTERMAIN_H

#include <QtCore/qobject.h>
#include <QtCore/qchar.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include "../../src/ccferror.h"
#include "converterflags.h"

class ConverterCore : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit ConverterCore(const QStringList &args, QObject *parent = 0);
    void convert();

private:
    void setFlags(const QStringList &args);
    void displayHelp();

    bool helpMode;
    ConverterFlags *flags;
};

#endif // CONVERTERMAIN_H
