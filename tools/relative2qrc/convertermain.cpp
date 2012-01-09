#include <QtCore/qcoreapplication.h>
#include "convertercore.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ConverterCore converter(a.arguments());
    if (!converter.isErrorState()) {
        converter.convert();
    }

    if (converter.isErrorState())
        qFatal(converter.errorMessage().toLocal8Bit());

    return 0;//    a.exec();
}
