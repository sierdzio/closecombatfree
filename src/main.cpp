#include <QApplication>
#include "ccfmain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CcfMain *viewer = new CcfMain();
    if (!viewer->isErrorState()) {
        viewer->setSource(QUrl("qml/main.qml"));
        if (viewer->isConfigMaximised()) {
            viewer->showMaximized();
        } else {
            viewer->show();
        }
        return a.exec();
    } else {
        qFatal(viewer->errorMessage().toLocal8Bit());
        return 1;
    }
}
