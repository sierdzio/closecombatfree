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

#include <QtGui/QGuiApplication>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>

#include "ccfmain.h"
#include "qmlBase/ccfqmlbasescenario.h"
#include "qmlBase/ccfqmlbaseunit.h"

// Singleton
CcfMain *CcfMain::m_instance = NULL;

/*!
  Returns instance of CcfMain class. Creates a new one if none exist.
  Only one instance of this class is allowed. Optionally takes in a command line
  parser object \a cmd, so that code execution can be influenced by command line
  arguments.
 */
CcfMain *CcfMain::instance(CcfCommandLineParser *cmd)
{
    if (!m_instance)
        m_instance = new CcfMain(cmd, 0);
    return m_instance;
}

/*!
  Main view's constructor, full of important stuff.

  Sets all global properties, sizing policy, connects important
  signals and slots, reads config file(s).
  */
CcfMain::CcfMain(CcfCommandLineParser *cmd, QWindow *parent) :
    QQuickView(parent), CcfError(), m_cmdLnParser(cmd)
{
    qmlRegisterType<CcfQmlBaseScenario>("QmlBase", 0, 1, "BaseScenario");
    qmlRegisterType<CcfQmlBaseUnit>("QmlBase", 0, 1, "BaseUnit");

    m_logger = new CcfLogger(this, m_cmdLnParser->isDebug());
    m_global = new CcfGlobal(this, m_logger);
    m_gameManager = new CcfGameManager(this);
    m_terrain = new CcfTerrain(this);
    m_engineHelpers = new CcfEngineHelpers(this);
    m_scenarioState = new CcfScenarioState(this);
    initConfiguration();

    rootContext()->setContextProperty("Global", m_global);
    rootContext()->setContextProperty("Config", m_configuration);
    rootContext()->setContextProperty("GameManager", m_gameManager);
    rootContext()->setContextProperty("Terrain", m_terrain);
    rootContext()->setContextProperty("EngineHelpers", m_engineHelpers);
    rootContext()->setContextProperty("ScenarioState", m_scenarioState);
    rootContext()->setContextProperty("Logger", m_logger);

    QString pwd = qApp->applicationDirPath() + "/";
    rootContext()->setContextProperty("PWD", pwd);

    setResizeMode(QQuickView::SizeRootObjectToView);
//    connect(this, SIGNAL(sceneResized(QSize)), configuration, SLOT(windowResized(QSize)));
    connect(m_configuration, SIGNAL(sizeModifiedInGame(int,int)), this, SLOT(forceViewportResize(int,int)));
    connect(engine(), SIGNAL(quit()), this, SLOT(quit()));
    connect(m_configuration, SIGNAL(maximise()), this, SLOT(showMaximized()));
    connect(m_configuration, SIGNAL(demaximise()), this, SLOT(showNormal()));
    connect(m_global, SIGNAL(disableQrc(QObject*)), this, SLOT(disableQrc(QObject*)));
}

/*!
  Returns true if config is set to be maximised.
  */
bool CcfMain::isConfigMaximised()
{
    return m_configuration->isMaximised();
}

/*!
  Orders the view to be resized.
  */
void CcfMain::resizeView(QSize newSize)
{
    m_configuration->windowResized(newSize);
}

/*!
  Returns a pointer to CcfLogger instance. Meant for logging, of course.
 */
CcfLogger *CcfMain::logger()
{
    return m_logger;
}

/*!
  Slot invoked when application is ordered to stop.

  This is not called when user terminates by the 'x' button on windowing
  system, or using process manager.
  */
void CcfMain::quit()
{
    m_configuration->saveConfig();
    if (m_configuration->isErrorState()) {
        qWarning(qPrintable(m_configuration->errorMessage()), NULL);
    }

    delete m_configuration;
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
    m_configuration = new CcfConfig("config", m_global, this);
    if (m_configuration->isErrorState()) {
        printf("Error while reading configuration file! Message: %s\n",
               qPrintable(m_configuration->errorMessage()));
        printf("Loading default configuration... ");
        delete m_configuration;
        m_configuration = new CcfConfig("config_default", m_global, this);

        if (m_configuration->isErrorState()) {
            printf("ERROR: %s\n", qPrintable(m_configuration->errorMessage()));
            return false;
        } else {
            printf("OK\n");
            return true;
        }
    }
    return true;
}
