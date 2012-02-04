/****************************************************************************
** Close Combat Free is a free, easily moddable CC-like game.
** Copyright (C) 2011 Tomasz Siekierda
** Email: sierdzio@gmail.com, website: http://www.sierdzio.com
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program (in doc/ directory).
** If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
****************************************************************************/

#include "ccfmain.h"

/*!
  \ingroup CloseCombatFree
  @{
 */

CcfMain::CcfMain(QWidget *parent) :
    QDeclarativeView(parent), CcfError()
{
    global = new CcfGlobal(this);
    gameManager = new CcfGameManager(this);
    terrain = new CcfTerrain(this);
    initConfiguration();

    rootContext()->setContextProperty("Global", global);
    rootContext()->setContextProperty("Config", configuration);
    rootContext()->setContextProperty("GameManager", gameManager);
    rootContext()->setContextProperty("Terrain", terrain);

    QString pwd = qApp->applicationDirPath() + "/";
    rootContext()->setContextProperty("PWD", pwd);
//    engine()->addImportPath("saves/");
//    qDebug() << engine()->importPathList();

    setResizeMode(QDeclarativeView::SizeRootObjectToView);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    viewport()->setAttribute(Qt::WA_NoSystemBackground);
    connect(this, SIGNAL(sceneResized(QSize)), configuration, SLOT(windowResized(QSize)));
    connect(configuration, SIGNAL(sizeModifiedInGame(int,int)), this, SLOT(forceViewportResize(int,int)));
    connect(engine(), SIGNAL(quit()), this, SLOT(quit()));
    connect(configuration, SIGNAL(maximise()), this, SLOT(showMaximized()));
    connect(configuration, SIGNAL(demaximise()), this, SLOT(showNormal()));
    connect(global, SIGNAL(disableQrc(QObject*)), this, SLOT(disableQrc(QObject*)));
}

bool CcfMain::isConfigMaximised()
{
    return configuration->maximised();
}

void CcfMain::resizeView(QSize newSize)
{
    configuration->windowResized(newSize);
}

void CcfMain::quit()
{
    configuration->saveConfig();
    if (configuration->isErrorState()) {
        qWarning(configuration->errorMessage().toLocal8Bit());
    }

    delete configuration;
    qApp->quit();
}

void CcfMain::forceViewportResize(int width, int height)
{
    setGeometry(x(), y(), width, height);
}

void CcfMain::disableQrc(QObject *object)
{
    QDeclarativeContext *context = engine()->contextForObject(object);
    context->setBaseUrl(QUrl::fromLocalFile(""));
}

bool CcfMain::initConfiguration()
{
    configuration = new CcfConfig("config", global, this);
    if (configuration->isErrorState()) {
        printf("Error while reading configuration file! Message: "
               + configuration->errorMessage().toLocal8Bit() + "\n");
        printf("Loading default configuration... ");
        configuration = new CcfConfig("config_default", global, this);

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

/*! @}*/
