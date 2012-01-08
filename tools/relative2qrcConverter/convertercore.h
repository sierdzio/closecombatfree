#ifndef CONVERTERCORE_H
#define CONVERTERCORE_H

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include "../../src/ccferror.h"
#include "converterflags.h"
#include "converterfile.h"

class ConverterCore : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit ConverterCore(QObject *parent = 0);

private:
    void init();

    ConverterFlags *flags;
};

#endif // CONVERTERCORE_H
