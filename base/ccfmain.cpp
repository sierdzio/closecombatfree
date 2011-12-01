#include "ccfmain.h"

CcfMain::CcfMain(QWidget *parent) :
    QDeclarativeView(parent)
{
    configuration = new CcfConfig("config", this);
    if (configuration->isErrorState()) {
        printf("Error while reading configuration file! Message: "
               + configuration->errorMessage().toLocal8Bit() + "\n");
        printf("Loading default configuration... ");
        configuration = new CcfConfig("config_default", this);

        if (configuration->isErrorState()) {
            printf("ERROR: " + configuration->errorMessage().toLocal8Bit() + "\n");
            return;
        } else {
            printf("OK\n");
        }
    }
    this->rootContext()->setContextObject(configuration);

    QString pwd = qApp->applicationDirPath() + "/";
    this->rootContext()->setContextProperty("PWD", pwd);

    this->setSource(QUrl("base/main.qml"));
    this->setAttribute(Qt::WA_OpaquePaintEvent);
    this->setAttribute(Qt::WA_NoSystemBackground);
    this->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    this->viewport()->setAttribute(Qt::WA_NoSystemBackground);
    connect(this, SIGNAL(sceneResized(QSize)), configuration, SLOT(windowResized(QSize)));
    connect(this->engine(), SIGNAL(quit()), this, SLOT(quit()));
}

void CcfMain::quit()
{
    configuration->saveConfig();
    if (configuration->isErrorState()) {
        qDebug() << configuration->errorMessage();
    }

    delete configuration;
    qApp->quit();
}
