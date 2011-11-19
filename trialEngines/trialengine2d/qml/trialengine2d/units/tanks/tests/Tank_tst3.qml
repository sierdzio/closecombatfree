import QtQuick 1.1
import "../"

Tank {
    id: root
    unitType: "Test tank 3"
    unitLogo: "../img/units/tanks/generic/tank_tst3_logo.png"
    rotationSpeed: 40
    turretRotationSpeed: 35
    turretSize: 44
    maxSpeed: 20
    acceleration: 2
    unitWidth: 50
    unitHeight: 80

    hullColor: "#505050"

    Tank_tst3_hull {
        id: hull
    }

    TurretImage {
        id: turret
        bodyTexture: "../../img/units/tanks/generic/tank_tst3_turret_main.png"
        barrelBaseTexture: "../../img/units/tanks/generic/turret_barrelBase.png"
        barrelSegment1Texture: "../../img/units/tanks/generic/turret_barrelSegment1_tank_tst3.png"
    }
}
