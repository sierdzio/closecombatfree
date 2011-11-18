import QtQuick 1.1
//import Qt.labs.particles 1.0
import "../"
//import "../../../gui"
//import "../../../engineLogicHelpers.js" as Logic
//import "../../engineActionLogic.js" as ActionLogic

Tank {
    id: root
    unitType: "Test tank 1"
    unitLogo: "../img/units/tanks/generic/tank_tst1_logo.png"
    tankHull: "tests/Tank_tst1_hull.qml"
    tankTurret: "tests/Tank_tst1_turret.qml"
//    tankTurret: "../Turret.qml"
    rotationSpeed: 40
    turretRotationSpeed: 35
    maxSpeed: 20
    acceleration: 2
    unitWidth: 62
    unitHeight: 100
}
