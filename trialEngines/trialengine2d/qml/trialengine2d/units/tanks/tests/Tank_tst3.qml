import QtQuick 1.1
import "../"

Tank {
    id: root
    unitType: "Test tank 3"
    unitLogo: "../img/units/tanks/generic/tank_tst2_logo.png"
    tankHull: "tests/Tank_tst1_hull.qml"
    tankTurret: "tests/Tank_tst1_turret.qml"
    rotationSpeed: 40
    turretRotationSpeed: 35
    maxSpeed: 20
    acceleration: 2
    unitWidth: 62
    unitHeight: 100
}
