import QtQuick 1.1
import Qt.labs.particles 1.0
import "../"
import "../../gui"
import "../../engineLogicHelpers.js" as Logic
import "../engineActionLogic.js" as ActionLogic

Unit {
    // Tank properties.
    property int frontArmour: 100 // mm
    property int sideArmour: 80 // mm
    property int backArmour: 60 // mm
    property int turretArmout: 80 // mm - might evolve into {front, side, back} triplet, too

    // Additional properties, not important for non-developers/ modders.
    property int turretRotation: 0
    property string turretBodyTexture: "../../img/units/tanks/generic/tank_tst1_turret_main.png"
    property int turretSize: 60
    property color hullColor: "#7b8259"
    property int defenceSphereRotation: 0
    property string defenceSphereColor: ""

    onMoveTo: {
        exhaust.burst(20);
        exhaustLater.burst(40);
    }

    signal fireTo (real targetX, real targetY)
    onFireTo: ActionLogic.turretFireTo(targetX, targetY);

    signal smokeTo (real targetX, real targetY)
    onSmokeTo: ActionLogic.turretSmokeTo(targetX, targetY);

    id: root
    unitType: "Test tank 1"
    unitLogo: "../img/units/tanks/generic/tank_tst1_logo.png"
    rotationSpeed: 40
    turretRotationSpeed: 35
    maxSpeed: 20
    acceleration: 2
    unitWidth: 62
    unitHeight: 100

    Tank_tst1_hull {
        id: hull
        anchors.top: parent.top
        anchors.left: parent.left
        hullColor: root.hullColor
        tankWidth: root.unitWidth
        tankHeight: root.unitHeight
    }

    Tank_tst1_turret {
        property bool __firing: false
        property bool __smoking: false

        id: turret
        turretBodySize: turretSize
        anchors.top: hull.top
        anchors.left: hull.left
        anchors.topMargin: hull.centerY - turret.centerY
        anchors.leftMargin: hull.centerX - turret.centerX
        bodyTexture: turretBodyTexture

        transform: Rotation {
            origin.x: turret.centerX; origin.y: turret.centerY; angle: turretRotation
        }
    }

    Behavior on turretRotation {
        SequentialAnimation {
            RotationAnimation {
                id: turretRotationAnimation
                duration: 3000
                direction: RotationAnimation.Shortest
                easing.type: Easing.InOutQuad
            }
            ScriptAction {
                script: {
                    if (turret.__firing) {
                        turret.firing = true;
                        turret.__firing = false;
                        actionFinished(__tempX, __tempY);
                        changeStatus("READY");
                    } else if (turret.__smoking) {
                        turret.smoking = true;
                        turret.__smoking = false;
                        actionFinished(__tempX, __tempY);
                        changeStatus("READY");
                    }
                }
            }
        }
    }

    Particles {
        id: exhaust

        width: 1; height: 1
        x: (2 * centerX) - 3
        y: (2 * centerY) - 3

        emissionRate: 0; emissionVariance: 6
        lifeSpan: 600; lifeSpanDeviation: 800
        angle: rotation + 90; angleDeviation: 60;
        velocity: 5; velocityDeviation: 10
        source: "../../img/effects/vehicle_smoke.png"
    }

    Particles {
        id: exhaustLater

        width: 1; height: 1
        x: (2 * centerX) - 3
        y: (2 * centerY) - 3

        emissionRate: 0; emissionVariance: 3
        lifeSpan: 1000; lifeSpanDeviation: 400
        angle: rotation + 90; angleDeviation: 60;
        velocity: 40; velocityDeviation: 60
        source: "../../img/effects/vehicle_smoke.png"
    }
}
