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
import "../units/tanks/tests"
import "../qml/units"

Item {
    property string mapFile: "maps/Map_tst1.qml"

    id: root

    Tank_tst1 {
        objectName: "tank1"
        x: 550
        y: 400

        Component.onCompleted: {
            fireTo(650, 300);
            smokeTo(700, 250);
            fireTo(750, 200);
        }
    }

    Tank_tst2 {
        objectName: "tank2"
        x: 1100
        y: 400
    }

    Tank_tst3 {
        objectName: "tank3"
        x: 450
        y: 400
        hullColor: "#ffeebb"
    }

    Tank_tst3 {
        objectName: "tank4"
        x: 750
        y: 400
        rotation: 0

        Component.onCompleted: {
            queueOrder ("Move", 700, 300);
            queueOrder ("Move", 500, 250);
            queueOrder ("Attack", 50, 50);
        }
    }

    Tank_tst2 {
        objectName: "tank5"
        x: 950
        y: 400
        rotation: 0

        Component.onCompleted: {
            queueOrder ("Move", 700, 600);
            queueOrder ("Move fast", 500, 650);
            queueOrder ("Attack", 50, 50);
        }
    }
}
