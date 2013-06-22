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
import QtQuick.Particles 2.0
import QmlBase 0.1

import "../../../qml/units"
import "../../../qml/units/tanks"

Tank {
    id: root
    unitFileName: "Tank_tst3"
    unitType: "Test tank 3"
    unitLogo: "../../../img/units/tanks/generic/tank_tst3_logo.png"
    rotationSpeed: 55
    turretRotationSpeed: 55
    turretSize: 44
    maxSpeed: 60
    acceleration: 4
    unitWidth: 50
    unitHeight: 80

    hullColor: "#505050"

    Tank_tst3_hull {
        id: hull
    }

    TurretImage {
        id: turret
        bodyTexture: "../../../img/units/tanks/generic/tank_tst3_turret_main.png"
        barrelBaseTexture: "../../../img/units/tanks/generic/turret_barrelBase.png"
        barrelSegment1Texture: "../../../img/units/tanks/generic/turret_barrelSegment1_tank_tst3.png"
    }

//    soldiers: [
    Soldier {
        name: "Thomas Glory"
        role: "Commander"
    }

    Soldier {
        name: "Marco Ponati"
        role: "Gunner"
    }

    Soldier {
        name: "Kevin Niven"
        role: "Loader"
    }

    Soldier {
        name: "Gustav Store"
        role: "MG gunner"
    }
//    ]

/*
    Particles {
        id: destroyedFire

        width: 1; height: 1
        x: (centerX)
        y: (centerY)
        z: root.z + 4

        emissionRate: 0; emissionVariance: 3
        lifeSpan: 500; lifeSpanDeviation: 800
        angle: 0; angleDeviation: 360;
        velocity: 80; velocityDeviation: 50
        source: "../../../img/effects/vehicle_fire.png"
    }
*/
    states: [
        State {
            name: "destroyed"
            extend: "damaged_base"
//            StateChangeScript {
//                name: "destroyed_script"
//                script: { destroyedFire.burst(300) }
//            }
        }
    ]
}
