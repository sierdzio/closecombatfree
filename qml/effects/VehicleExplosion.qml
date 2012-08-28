import QtQuick 2.0
import QtQuick.Particles 2.0

Item {
    id: root

    function burst(amount) {
        fireEmitter.burst(10);
        afterEffect();
    }

    function pulse(duration) {
        fireEmitter.pulse(500);
        afterEffect();
    }

    function afterEffect() {
        fireballImage.makeFireball();
        explodeEmitter.pulse(800);
        smoke.pulse(20000, true);
    }

    /*!
      Starts a default explosion animation.
      */
    function run() {
        fireEmitter.pulse(800);
        afterEffect();
    }

    ParticleSystem {
        id: destroyedParticleSystem
        anchors.fill: parent

        ParticleGroup {
            name: "fire"
            duration: 1000

            Friction {
                anchors.fill: parent
                groups: ["fire"]
                factor: 4.4
            }

            TrailEmitter {
                id: smokeTrails
                group: "smokeTrails"
                emitRatePerParticle: 12
                lifeSpan: 300
                maximumEmitted: 20
                size: 15
            }
        }

        Emitter {
            id: fireEmitter
            group: "fire"
            enabled: false
            z: smokeTrails.z + 1

            width: 10
            height: 10
            anchors.centerIn: parent

            emitRate: 10
            maximumEmitted: 30
            lifeSpan: 1200
            lifeSpanVariation: 300
            velocity: AngleDirection {
                magnitude: 300
                angleVariation: 360
            }

            acceleration: PointDirection {xVariation: 20; yVariation: 20;}
            size: 10
        }

        ImageParticle {
            z: smokeParticle.z + 1
            source: "../../img/effects/gun_fire_particle1.png"
            groups: ["fire"]
        }

        ImageParticle {
            id: smokeParticle
            source: "../../img/effects/vehicle_smoke.png"
            groups: ["smokeTrails"]
            entryEffect: ImageParticle.Scale
        }
    }

    Image {
        id: fireballImage
        width: 2
        height: 2
        scale: 1
        visible: false
        anchors.centerIn: parent
        source: "../../img/effects/vehicle_fire.png"

        NumberAnimation {
            id: fireballAnimation
            from: 1
            to: 60
            target: fireballImage
            properties: "scale"
            duration: 300

            onRunningChanged: {
                if (running == false)
                    fireballImage.visible = false;
            }
        }

        function makeFireball() {
            fireballImage.visible = true;
            fireballAnimation.start();
        }
    }

    ParticleSystem {
        id: explodeParticleSystem
        anchors.fill: parent

        Emitter {
            id: explodeEmitter
            enabled: false

            anchors.centerIn: parent
            width: 200
            height: 200

            emitRate: 120
            shape: EllipseShape{}
            lifeSpan: 150
            lifeSpanVariation: 100
            size: 7
        }

        ImageParticle {
            source: "../../img/effects/vehicle_smoke_flash2.png"
            color: "#ffffffff"
            colorVariation: 0.2
            entryEffect: ImageParticle.Scale
        }
    }

    VehicleExhaust {
        id: smoke
        anchors.fill: parent
    }
}
