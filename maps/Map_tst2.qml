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

import QtQuick 2.1
import QmlBase 0.1
import "../qml/maps"
import "../qml/maps/props"
import "../maps/props"
import "../maps"

Map {
    backgroundImage: "img/maps/test2.png"
    hipsometricImage: "img/maps/hipsometric_test1.png"

    HouseBasic_tst {
        id: house1
        x: 20
        y: 50
    }

    Tree_tst {
        x: 220
        y: 30
    }

    Tree_tst {
        x: 250
        y: 30
    }

    Tree_tst {
        x: 280
        y: 30
    }

    PropCluster {
        x: 500
        y: 30
        numberOfObjects: 10
        baseElement: "../../../maps/props/Tree_tst.qml"
    }

    Rectangle {
        objectName: "Gray rectangle"
        id: rect2
        x: 360
        y: 50
        width: 100
        height: 100
        color: "#aabbbbcc"
        opacity: propOpacity
    }

    Rectangle {
        objectName: "Shallow water"
        id: rect3
        x: 905
        y: 162
        width: 150
        height: 300
        radius: 50
        color: "#ee1188ee"
        opacity: propOpacity
    }
}
