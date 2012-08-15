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

#ifndef CCFMAIN_H
#define CCFMAIN_H

#include <QtCore/QObject>
#include <QtCore/QSize>
#include <QtGui/QWindow>
#include <QtQuick/QQuickView>

#include "config/ccfconfig.h"
#include "ccfglobal.h"
#include "ccfgamemanager.h"
#include "ccfterrain.h"

/*!
  \ingroup CloseCombatFree
  @{
 */

/*!
  Class responsible for displaying the game content on screen,
  connecting other C++ code to the game engine etc.
  */
class CcfMain : public QQuickView, public CcfError
{
    Q_OBJECT
public:
    explicit CcfMain(QWindow *parent = 0);
    bool isConfigMaximised();
    void resizeView(QSize newSize);

public slots:
    void quit();
    void forceViewportResize(int width, int height);
    void disableQrc(QObject *object);

private:
    bool initConfiguration();

    CcfConfig *configuration;
    CcfGlobal *global;
    CcfGameManager *gameManager;
    CcfTerrain *terrain;
    QString pwd;
};

/*! @}*/

#endif // CCFMAIN_H
