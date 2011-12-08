#include <QApplication>
#include <QDebug>
#include "ccfmain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CcfMain *viewer = new CcfMain();
    if (!viewer->isErrorState()) {
        viewer->setSource(QUrl("qml/main.qml"));
        viewer->show();
        return a.exec();
    } else {
        qDebug() << viewer->errorMessage();
        return 1;
    }
}
