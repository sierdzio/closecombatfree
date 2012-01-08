#include <QtCore/qcoreapplication.h>
#include "convertermain.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ConverterMain converter(a.arguments());
    if (!converter.isErrorState()) {
        converter.convert();
    }

    if (converter.isErrorState())
        qFatal(converter.errorMessage().toLocal8Bit());

    return 0;//    a.exec();
}
