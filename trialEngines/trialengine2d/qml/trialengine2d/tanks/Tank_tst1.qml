import QtQuick 1.1
import Qt.labs.particles 1.0
import "../gui"
import "../engineLogicHelpers.js" as Logic

Item {
    // Tank properties.
    property string unitType: "Test tank 1"
    property int rotationSpeed: 40 // seconds needed for half rotation (180 deg)
    property int turretRotationSpeed: 35 // seconds needed for half rotation (180 deg)
    property int maxSpeed: 20 // km/h - not really, this will be tweaked later
    property int acceleration: 2 // m/s^2 (maybe... could be arbitrary - to be decided later)
    property int frontArmour: 100 // mm
    property int sideArmour: 80 // mm
    property int backArmour: 60 // mm
    property int turretArmout: 80 // mm - might evolve into {front, side, back} triplet, too
    property int tankWidth: 62 // mm - should be properly recalculated later on
    property int tankHeight: 100 // mm - should be properly recalculated later on

    // Additional properties, not important for non-developers/ modders.
    property int centerX: tankWidth/2
    property int centerY: tankHeight/2
    property int turretRotation: 0
    property string turretBodyTexture: "../img/tanks/generic/tank_tst1_turret_main.png"
    property int turretSize: 60
    property color hullColor: "#7b8259"
    property int defenceSphereRotation: 0
    property string defenceSphereColor: ""
    property string unitLogo: "../img/tanks/generic/tank_tst1_logo.png"

    signal moveTo (real newX, real newY)
    onMoveTo: {
        xMoveAnimation.__tempX = newX - (centerX);
        yMoveAnimation.__tempY = newY - (centerY);

        var newRotation = Logic.rotationAngle(x, y,
                                              xMoveAnimation.__tempX,
                                              yMoveAnimation.__tempY);
        rotationAnimation.duration = Logic.rotationDuration(rotation,
                                                            newRotation, rotationSpeed);
        rotation = newRotation;

        var moveDuration = Logic.targetDistance(x, y,
                                                xMoveAnimation.__tempX,
                                                yMoveAnimation.__tempY) * 800 / (maxSpeed);
        xMoveAnimation.duration = moveDuration;
        yMoveAnimation.duration = moveDuration;

        exhaust.burst(20);
        exhaustLater.burst(40);
    }
    signal fireTo (real targetX, real targetY)
    onFireTo: {
        xMoveAnimation.__tempX = targetX;
        yMoveAnimation.__tempY = targetY;
        var newRotation = Logic.rotationAngle(x, y,
                                              targetX - centerX,
                                              targetY - centerY) - rotation;
        turretRotationAnimation.duration = Logic.rotationDuration(turretRotation,
                                                                  newRotation,
                                                                  turretRotationSpeed);
        turretRotation = newRotation;
        turret.__firing = true;
    }
    signal actionFinished(real targetX, real targetY)

    id: root
    width: tankWidth
    height: tankHeight

    Tank_tst1_hull {
        id: hull
        anchors.top: parent.top
        anchors.left: parent.left
        hullColor: root.hullColor
        tankWidth: root.tankWidth
        tankHeight: root.tankHeight
    }

    Tank_tst1_turret {
        property bool __firing: false

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

    DefenceSphere {
        id: defenceSphere
        z: hull.z - 1

        visible: {
            if (defenceSphereColor == "") {
                return false;
            } else {
                return true;
            }
        }
        imageSource: {
            if (defenceSphereColor == "")
                return "";
            else
                return "../img/gui/defenceSphere_" + defenceSphereColor + ".png"

        }
        anchors {
            top: hull.top
            left: hull.left
            topMargin: 0
            leftMargin: centerX
        }

        transform: Rotation {
            origin.x: 0
            origin.y: defenceSphere.height/2
            angle: defenceSphereRotation
        }
    }

    Behavior on rotation {
        RotationAnimation {
            id: rotationAnimation
            property: "rotation"
            duration: 2000
            direction: RotationAnimation.Shortest
            easing.type: Easing.InOutQuad
            onRunningChanged: { // Not sure whether doing that will be good under all circumstances
                if (!rotationAnimation.running) {
                    x = xMoveAnimation.__tempX;
                    y = yMoveAnimation.__tempY;
                }
            }
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
                        actionFinished(xMoveAnimation.__tempX, yMoveAnimation.__tempY);
                    }
                }
            }
        }
    }
    Behavior on x {
        NumberAnimation {
            property int __tempX: x

            id: xMoveAnimation
            duration: 2500
            easing.type: Easing.InOutQuad
        }
    }
    Behavior on y {
        NumberAnimation {
            property int __tempY: y

            id: yMoveAnimation
            duration: 2500
            easing.type: Easing.InOutQuad
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
        source: "../img/effects/vehicle_smoke.png"
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
        source: "../img/effects/vehicle_smoke.png"
    }
}
