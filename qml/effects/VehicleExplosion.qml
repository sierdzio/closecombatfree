import QtQuick 2.0
import QtQuick.Particles 2.0

Item {
    id: root

    function burst(amount) {
        fireEmitter.burst(amount);
        afterEffect();
    }

    function pulse(duration) {
        fireEmitter.pulse(duration);
        afterEffect();
    }

    function afterEffect() {
        fireballImage.makeFireball();
        explodeEmitter.pulse(800);
        smoke.pulse(10000, true);
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
                factor: 0.2
            }

            TrailEmitter {
                group: "smokeTrails"
                emitRatePerParticle: 10
                lifeSpan: 100
                maximumEmitted: 60
                size: 10
            }
        }

        Turbulence {
            anchors.fill: parent
            groups: ["smokeTrails"]
            strength: 20
        }

        Emitter {
            id: fireEmitter
            group: "fire"
            enabled: false

            width: 10
            height: 10
            anchors.centerIn: parent

            emitRate: 80
            lifeSpan: 300
            lifeSpanVariation: 300
            velocity: AngleDirection {
                magnitude: 180
                angleVariation: 360
            }

            acceleration: PointDirection {xVariation: 20; yVariation: 20;}
            size: 20
        }

        ImageParticle {
            source: "../../img/effects/vehicle_fire.png"
            redVariation: 0.6
            color: "#aa5511"
            groups: ["fire"]
        }

        ImageParticle {
            source: "../../img/effects/vehicle_smoke.png"
//            source: "../../img/effects/vehicle_smoke_light.png"
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

            emitRate: 200
            shape: EllipseShape{}
            lifeSpan: 300
            lifeSpanVariation: 150
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
