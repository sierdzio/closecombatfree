#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QDebug>
#include "ccfconfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDeclarativeView *viewer = new QDeclarativeView();

    CcfConfig configuration("config");
    if (configuration.isErrorState()) {
        qDebug() << "Error while reading configuration! Message: "
                 << configuration.errorMessage();
    }/* else {
        qDebug() << configuration.configurationString();
    }*/
    viewer->rootContext()->setContextObject(&configuration);

    QString pwd = a.applicationDirPath() + "/";
    viewer->rootContext()->setContextProperty("PWD", pwd);

    viewer->rootContext()->setContextProperty("windowWidth", configuration.configWindowWidth());
    viewer->rootContext()->setContextProperty("windowHeight", configuration.configWindowHeight());

    viewer->setSource(QUrl("base/main.qml"));
    viewer->setAttribute(Qt::WA_OpaquePaintEvent);
    viewer->setAttribute(Qt::WA_NoSystemBackground);
    viewer->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    viewer->viewport()->setAttribute(Qt::WA_NoSystemBackground);

    viewer->show();
    return a.exec();
}
