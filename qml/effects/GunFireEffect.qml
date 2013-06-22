import QtQuick 2.1
import QtQuick.Particles 2.0

Item {
    id: root

    function burst(amount) {
        fire.burst(amount);
    }

    function pulse(duration, verbose) {
        if (verbose === true) {
            fire.lifeSpan = 1500;
            angle.angleVariation = 30;
        }

        fire.pulse(duration);
        trailEmitter.pulse(1500);
    }

    ImageParticle {
        source: "../../img/effects/vehicle_smoke_flash.png"
        width: 30
        height: 30
        groups: "B"
        system: particleSystem
        entryEffect: ImageParticle.Fade
    }

    Emitter {
        id: trailEmitter
        enabled: false
        group: "B"
        system: particleSystem

        lifeSpan: 600
        lifeSpanVariation: 800
        velocity: AngleDirection {
            magnitude: 15
            angle: 270
            angleVariation: 30
        }
    }

    ParticleSystem {
        id: particleSystem
        anchors.fill: parent

        Emitter {
            id: fire
            enabled: false
            width: 3
            height: 3
            anchors.centerIn: parent
            group: "A"

            lifeSpan: 80
            lifeSpanVariation: 20
            velocity: AngleDirection {
                id: angle
                magnitude: 2
                angle: 270
                angleVariation: 10
            }

            acceleration: PointDirection {xVariation: 10; yVariation: 10;}
            velocityFromMovement: -22
            size: 30
        }

        ImageParticle {
            source: "../../img/effects/gun_fire_particle1.png"
            width: 30
            height: 30
            groups: "A"
            entryEffect: ImageParticle.Fade
            alpha: 0.4
        }
    }
}
