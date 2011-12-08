import QtQuick 1.1
import "../../../qml/units"
import "../../../qml/units/tanks"

Tank {
    id: root
    unitType: "Test tank 3"
    unitLogo: PWD + "img/units/tanks/generic/tank_tst3_logo.png"
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
        bodyTexture: PWD + "img/units/tanks/generic/tank_tst3_turret_main.png"
        barrelBaseTexture: PWD + "img/units/tanks/generic/turret_barrelBase.png"
        barrelSegment1Texture: PWD + "img/units/tanks/generic/turret_barrelSegment1_tank_tst3.png"
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
}
