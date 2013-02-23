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

#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>

#include "ccfmain.h"
#include "qmlBase/ccfqmlbaserostermenu.h"
#include "qmlBase/ccfqmlbasescenario.h"
#include "qmlBase/ccfqmlbasemap.h"
#include "qmlBase/ccfqmlbaseunit.h"
#include "qmlBase/ccfqmlbasesoldier.h"

// Singleton
CcfMain *CcfMain::mInstance = NULL;

/*!
  Returns instance of CcfMain class. Creates a new one if none exist.
  Only one instance of this class is allowed. Optionally takes in a command line
  parser object \a cmd, so that code execution can be influenced by command line
  arguments.
 */
CcfMain *CcfMain::instance(CcfCommandLineParser *cmd)
{
    if (!mInstance)
        mInstance = new CcfMain(cmd, 0);
    return mInstance;
}

/*!
  Main view's constructor, full of important stuff.

  Sets all global properties, sizing policy, connects important
  signals and slots, reads config file(s).
  */
CcfMain::CcfMain(CcfCommandLineParser *cmd, QWindow *parent) :
    QQuickView(parent), CcfError(), mCmdLnParser(cmd)
{
    qmlRegisterType<CcfQmlBaseRosterMenu>("QmlBase", 0, 1, "BaseRosterMenu");
    qmlRegisterType<CcfQmlBaseScenario>("QmlBase", 0, 1, "BaseScenario");
    qmlRegisterType<CcfQmlBaseMap>("QmlBase", 0, 1, "BaseMap");
    qmlRegisterType<CcfQmlBaseUnit>("QmlBase", 0, 1, "BaseUnit");
    qmlRegisterType<CcfQmlBaseSoldier>("QmlBase", 0, 1, "Soldier");

    mLogger = new CcfLogger(this, mCmdLnParser->isDebug());
    mGlobal = new CcfGlobal(this);
    mGameManager = new CcfGameManager(this);
    mEngineHelpers = new CcfEngineHelpers(this);
    initConfiguration();

    rootContext()->setContextProperty("Global", mGlobal);
    rootContext()->setContextProperty("Config", mConfiguration);
    rootContext()->setContextProperty("GameManager", mGameManager);
    rootContext()->setContextProperty("EngineHelpers", mEngineHelpers);
    rootContext()->setContextProperty("Logger", mLogger);

    QString pwd = qApp->applicationDirPath() + "/";
    rootContext()->setContextProperty("PWD", pwd);

    setResizeMode(QQuickView::SizeRootObjectToView);
//    connect(this, SIGNAL(sceneResized(QSize)), configuration, SLOT(windowResized(QSize)));
    connect(mConfiguration, SIGNAL(sizeModifiedInGame(int,int)), this, SLOT(forceViewportResize(int,int)));
    connect(engine(), SIGNAL(quit()), this, SLOT(quit()));
    connect(mConfiguration, SIGNAL(maximise()), this, SLOT(showMaximized()));
    connect(mConfiguration, SIGNAL(demaximise()), this, SLOT(showNormal()));
    connect(mGlobal, SIGNAL(disableQrc(QObject*)), this, SLOT(disableQrc(QObject*)));
}

/*!
  Returns true if config is set to be maximised.
  */
bool CcfMain::isConfigMaximised()
{
    return mConfiguration->isMaximised();
}

/*!
  Orders the view to be resized.
  */
void CcfMain::resizeView(QSize newSize)
{
    mConfiguration->windowResized(newSize);
}

/*!
  Returns a pointer to CcfLogger instance. Meant for logging, of course.
 */
CcfLogger *CcfMain::logger()
{
    return mLogger;
}

/*!
  Returns a pointer to CcfConfig instance.
 */
CcfConfig *CcfMain::config()
{
    return mConfiguration;
}

/*!
  Returns a pointer to CcfGlobal instance.
 */
CcfGlobal *CcfMain::global()
{
    return mGlobal;
}

/*!
  Slot invoked when application is ordered to stop.

  This is not called when user terminates by the 'x' button on windowing
  system, or using process manager.
  */
void CcfMain::quit()
{
    mConfiguration->saveConfig();
    if (mConfiguration->isErrorState()) {
        qWarning(qPrintable(mConfiguration->errorMessage()), NULL);
    }

    delete mConfiguration;
    qApp->quit();
}

/*!
  Forces the window to be resized.

  Invoked when user changes window size in game preferences.
  */
void CcfMain::forceViewportResize(int width, int height)
{
    setGeometry(x(), y(), width, height);
}

/*!
  Disables QRC reading on a given QObject (usually a Loader or Repeater).

  Very important, enables loading of non-qrc paths in an app that uses QRC.
  Usually, if you load any QML code through QRC, all subsequent loads will
  also be performed through resource system - even when you specify a path that
  is not present in any resource file. This method counters this by clearing
  the "qrc mode".
  */
void CcfMain::disableQrc(QObject *object)
{
    QQmlContext *context = engine()->contextForObject(object);
    context->setBaseUrl(QUrl::fromLocalFile(""));
}

/*!
  Initialises the configuration, loading default on error.
  */
bool CcfMain::initConfiguration()
{
    mConfiguration = new CcfConfig("config", mLogger, this);
    if (mConfiguration->isErrorState()) {
        printf("Error while reading configuration file! Message: %s\n",
               qPrintable(mConfiguration->errorMessage()));
        printf("Loading default configuration... ");
        delete mConfiguration;
        mConfiguration = new CcfConfig("config_default", mLogger, this);

        if (mConfiguration->isErrorState()) {
            printf("ERROR: %s\n", qPrintable(mConfiguration->errorMessage()));
            return false;
        } else {
            printf("OK\n");
            return true;
        }
    }
    return true;
}
