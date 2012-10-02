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
import "../units/tanks/tests"
import "../qml/units"

Item {
    property string mapFile: "maps/Map_tst2.qml"

    id: root

    Tank_tst1 {
        objectName: "tank1"
        x: 150
        y: 400
    }

    Tank_tst3 {
        objectName: "tank2"
        x: 300
        y: 400
    }

    Tank_tst1 {
        objectName: "tank3"
        x: 450
        y: 400
        rotation: 90
        hullColor: "#ffffff"
    }

    Tank_tst1 {
        objectName: "tank4"
        x: 600
        y: 400
        rotation: 0
    }

    Tank_tst2 {
        objectName: "tank5"
        x: 750
        y: 400
        rotation: 0
    }

    Tank_tst3 {
        objectName: "tank6"
        x: 750
        y: 200
    }
}
