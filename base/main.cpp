#include <QtGui/QApplication>
#include "ccfmain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CcfMain *viewer = new CcfMain();
    viewer->show();
    return a.exec();
}
