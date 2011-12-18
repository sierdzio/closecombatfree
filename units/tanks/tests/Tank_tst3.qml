import QtQuick 1.1
import Qt.labs.particles 1.0
import "qrc:/core/units"
import "qrc:/core/units/tanks"

Tank {
    id: root
    unitType: "Test tank 3"
    unitLogo: "qrc:/img/units/tanks/generic/tank_tst3_logo.png"
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
        bodyTexture: "qrc:/img/units/tanks/generic/tank_tst3_turret_main.png"
        barrelBaseTexture: "qrc:/img/units/tanks/generic/turret_barrelBase.png"
        barrelSegment1Texture: "qrc:/img/units/tanks/generic/turret_barrelSegment1_tank_tst3.png"
    }

    soldiers: [
        Soldier {
            name: "Thomas Glory"
            role: "Commander"
        },
        Soldier {
            name: "Marco Ponati"
            role: "Gunner"
        },
        Soldier {
            name: "Kevin Niven"
            role: "Loader"
        },
        Soldier {
            name: "Gustav Store"
            role: "MG gunner"
        }
    ]

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
        source: "qrc:/img/effects/vehicle_fire.png"
    }

    states: [
        State {
            name: "destroyed"
            extend: "damaged_base"
            StateChangeScript {
                name: "destroyed_script"
                script: { destroyedFire.burst(300) }
            }
        }
    ]
}
