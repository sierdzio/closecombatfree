#ifndef CONVERTERMAIN_H
#define CONVERTERMAIN_H

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include "../../src/ccferror.h"
#include "converterflags.h"
#include "convertercore.h"

class ConverterMain : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit ConverterMain(const QStringList &args, QObject *parent = 0);
    void convert();

private:
    void setFlags();
    void displayHelp();

    ConverterFlags *flags;
};

#endif // CONVERTERMAIN_H
