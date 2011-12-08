import QtQuick 1.1
import "../../../qml/units"
import "../../../qml/units/tanks"

Tank {
    id: root
    unitType: "Test tank 1"
    unitLogo: PWD + "img/units/tanks/generic/tank_tst1_logo.png"
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

    soldiers: [
        Soldier {
            role: "Commander"
        },
        Soldier {
            role: "Gunner"
        },
        Soldier {
            role: "Loader"
        },
        Soldier {
            role: "Assistant"
        },
        Soldier {
            role: "Assistant"
        }
    ]
}
