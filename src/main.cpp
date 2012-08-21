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

#include "ccfmain.h"
#include "config/ccfcommandlineparser.h"

/*!
  \ingroup CloseCombatFree
  @{
 */

/*!
  CloseCombatFree's main routine.

  Responsible for preparing and running the application. Main QML file is
  specified here, as well as some general things (whether to show maximised or
  not, etc.).
  */
int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    CcfCommandLineParser cmd(a.arguments());

    // Check if --help was specified. If yes, print help and exit.
    if (cmd.wasHelpRequested()) {
        qFatal(qPrintable(cmd.helpMessage()), NULL);
        return 1;
    }

    CcfMain *viewer = new CcfMain(&cmd);
    if (!viewer->isErrorState()) {
        QUrl source = QUrl::fromLocalFile("qml/main.qml");
        viewer->setSource(source);
        if (viewer->isConfigMaximised()) {
            viewer->showMaximized();
        } else {
            viewer->show();
        }
        return a.exec();
    } else {
        qFatal(qPrintable(viewer->errorMessage()), NULL);
        return 1;
    }
}

/*! @}*/
