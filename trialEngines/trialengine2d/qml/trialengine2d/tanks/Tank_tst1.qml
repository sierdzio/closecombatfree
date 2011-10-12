import QtQuick 1.1
import Qt.labs.particles 1.0
import "../gui"
import "../engineLogicHelpers.js" as Logic

Item {
    property int centerX: hull.width/2
    property int centerY: hull.height/2
    property int turretRotation: 0
    property string turretBodyTexture: "../img/tanks/generic/tank_tst1_turret_main.png"
    property int defenceSphereRotation: 0
    property string defenceSphereColor: ""
    property int __tempX: x // Ugly, but I couldn't make it better, yet. Will retry later.
    property int __tempY: y // Ugly, but I couldn't make it better, yet. Will retry later.
    property bool __firing: false // Ugly, too.

    signal moveTo (real newX, real newY)
    onMoveTo: {
        /*if (x == newX) {
            y = newY;
        } else if (y == newY) {
            x = newX;
        } else*/ {
            __tempX = newX - (centerX);
            __tempY = newY - (centerY);
            rotation = Logic.rotationAngle(x, y, __tempX, __tempY);
        }
        exhaust.burst(20);
        exhaustLater.burst(40);
    }
    signal fireTo (real targetX, real targetY)
    onFireTo: {
        __tempX = targetX;
        __tempY = targetY;
        turretRotation = Logic.rotationAngle(x, y, targetX - centerX, targetY - centerY) - rotation;
        __firing = true;
    }
    signal actionFinished(real targetX, real targetY)

    id: root
    width: hull.width
    height: hull.height

    Tank_tst1_hull {
        id: hull
        anchors.top: parent.top
        anchors.left: parent.left
    }

    Tank_tst1_turret {
        id: turret
        anchors.top: hull.top
        anchors.left: hull.left
        anchors.topMargin: hull.centerY - turret.centerY
        anchors.leftMargin: 1
        bodyTexture: turretBodyTexture

        transform: Rotation { origin.x: turret.centerX; origin.y: turret.centerY; angle: turretRotation}
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
            duration: 2000; direction: RotationAnimation.Shortest
            onRunningChanged: { // Not sure whether doing that will be good under all circumstances
                if (!rotationAnimation.running) {
                    x = __tempX;
                    y = __tempY;
                }
            }
        }
    }
    Behavior on turretRotation {
        SequentialAnimation {
            RotationAnimation {
                duration: 3000
                direction: RotationAnimation.Shortest
            }
            ScriptAction {
                script: {
                    if (__firing) {
                        turret.firing = true;
                        __firing = false;
                        actionFinished(__tempX, __tempY);
                    }
                }
            }
        }
    }
    Behavior on x {
        NumberAnimation {
            duration: 2500; easing.type: Easing.InOutQuad
        }
    }
    Behavior on y {
        NumberAnimation {
            duration: 2500; easing.type: Easing.InOutQuad
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
