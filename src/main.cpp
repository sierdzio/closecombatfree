#include <QApplication>
#include <QDebug>
#include "ccfmain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CcfMain *viewer = new CcfMain();
    if (!viewer->isErrorState()) {
        viewer->setSource(QUrl("qrc:/qml/main.qml"));
        if (viewer->isConfigMaximised()) {
            viewer->showMaximized();
        } else {
            viewer->show();
        }
        return a.exec();
    } else {
        qDebug() << viewer->errorMessage();
        return 1;
    }
}
