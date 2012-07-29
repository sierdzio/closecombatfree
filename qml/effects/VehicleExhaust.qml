import QtQuick 2.0
import QtQuick.Particles 2.0

Item {
    id: root

    function burst(amount) {
        exhaust.burst(amount);
    }

    function pulse(duration, verbose) {
        if (verbose == true) {
            exhaust.lifeSpan = 1500;
            angle.angleVariation = 30;
        }

        exhaust.pulse(duration);
    }

    ParticleSystem {
        anchors.fill: parent

        Emitter {
            id: exhaust
            enabled: false
            width: 3
            height: 3
            anchors.centerIn: parent

            lifeSpan: 600
            lifeSpanVariation: 800
            velocity: AngleDirection {
                id: angle
                magnitude: 20
                angle: 90
                angleVariation: 10
            }

            acceleration: PointDirection {xVariation: 10; yVariation: 10;}
            velocityFromMovement: 12
            size: 20
        }

        Turbulence {
            anchors.fill: parent
            strength: 5
        }

        ImageParticle {
            source: "../../img/effects/vehicle_smoke.png"
            alphaVariation: 0.5
        }
    }
}
