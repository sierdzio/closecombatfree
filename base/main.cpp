#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include "ccfconfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDeclarativeView *viewer = new QDeclarativeView();

    CcfConfig *configuration = new CcfConfig("config");
    if (configuration->isErrorState()) {
        printf("Error while reading configuration file! Message: "
               + configuration->errorMessage().toLocal8Bit() + "\n");
        printf("Loading default configuration... ");
        configuration = new CcfConfig("config_default");

        if (configuration->isErrorState()) {
            printf("ERROR: " + configuration->errorMessage().toLocal8Bit() + "\n");
            return 1;
        } else {
            printf("OK\n");
        }
    }
    viewer->rootContext()->setContextObject(configuration);

    QString pwd = a.applicationDirPath() + "/";
    viewer->rootContext()->setContextProperty("PWD", pwd);

    viewer->setSource(QUrl("base/main.qml"));
    viewer->setAttribute(Qt::WA_OpaquePaintEvent);
    viewer->setAttribute(Qt::WA_NoSystemBackground);
    viewer->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    viewer->viewport()->setAttribute(Qt::WA_NoSystemBackground);
    QObject::connect(viewer, SIGNAL(sceneResized(QSize)), configuration, SLOT(windowResized(QSize)));

    viewer->show();
    return a.exec();
}