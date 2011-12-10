#include "ccfmain.h"

CcfMain::CcfMain(QWidget *parent) :
    QDeclarativeView(parent), CcfError()
{
//    if (initConfiguration() == true)
    initConfiguration();
    rootContext()->setContextObject(configuration);

//    QString pwd = qApp->applicationDirPath() + "/";
//    rootContext()->setContextProperty("PWD", pwd);

//    setSource(QUrl("base/main.qml"));
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    viewport()->setAttribute(Qt::WA_NoSystemBackground);
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

bool CcfMain::initConfiguration()
{
    configuration = new CcfConfig("config", this);
    if (configuration->isErrorState()) {
        printf("Error while reading configuration file! Message: "
               + configuration->errorMessage().toLocal8Bit() + "\n");
        printf("Loading default configuration... ");
        configuration = new CcfConfig("config_default", this);

        if (configuration->isErrorState()) {
            printf("ERROR: " + configuration->errorMessage().toLocal8Bit() + "\n");
            return false;
        } else {
            printf("OK\n");
            return true;
        }
    }
    return true;
}
