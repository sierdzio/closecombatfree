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
import QmlBase 0.1
import "../qml/maps"
import "../maps/props"
import "../maps"

Map {
    backgroundImage: "img/maps/test1.png"
    hipsometricImage: "img/maps/hipsometric_2sides.png"

    height: 1200
    width: 1200

    Rectangle {
        objectName: "Shallow water"
        id: rect3
        x: 200
        y: 700
        width: 50
        height: 350
        radius: 50
        color: "#ee1188ee"
        opacity: propOpacity
    }
}
