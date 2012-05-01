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

import QtQuick 2.0
import "../qml/gui"
import "../qml/units/engineActionLogic.js" as LogicHelpers
import "../qml/scenarios/engineScenarioLogic.js" as ScenarioLogic

// Elements available for editors.
import "../maps"
import "../maps/props"
import "../scenarios"
import "../saves"
import "../units"
import "../units/tanks"
import "../units/tanks/tests"
import "../qml"
import "../qml/maps"
import "../qml/maps/props"
import "../qml/scenarios"
import "../qml/units"
import "../qml/units/tanks"

Rectangle {
    property string playerSide: "neutral" // Experimental!
    property color menuBackgroundColor: "#7e8c24"

    id: root
    color: menuBackgroundColor

    width: map.width
    height: map.height

    Map {
        id: map
        hipsometricImage: "../../img/maps/hipsometric_default.png"
        backgroundImage: "../../img/maps/test1.jpg"
        anchors.fill: parent

        Tank_tst2 {
            id: tank_tst21
            x: 571
            y: 641
            centerY: 36
        }
    }
}

