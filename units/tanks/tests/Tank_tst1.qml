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

import "../../../qml/units"
import "../../../qml/units/tanks"

Tank {
    id: root
    unitFileName: "Tank_tst1"
    unitType: "Test tank 1"
    unitLogo: "../../../img/units/tanks/generic/tank_tst1_logo.png"
    rotationSpeed: 40
    turretRotationSpeed: 35
    maxSpeed: 20
    acceleration: 2
    unitWidth: 62
    unitHeight: 100

    Tank_tst1_hull {
        id: hull
    }

    Tank_tst1_turret {
        id: turret
    }

//    soldiers: [
    Soldier {
        role: "Commander"
    }

    Soldier {
        role: "Gunner"
    }

    Soldier {
        role: "Loader"
    }

    Soldier {
        role: "Assistant"
    }

    Soldier {
        role: "Assistant"
    }
//    ]
}
