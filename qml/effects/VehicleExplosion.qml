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
        fireballEmitter.burst(100);
        explodeEmitter.pulse(800);
        smoke.pulse(10000, true);
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
            speed: AngleDirection {
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
//            source: "../../img/effects/vehicle_smoke.png"
            source: "../../img/effects/vehicle_smoke_light.png"
            groups: ["smokeTrails"]
            entryEffect: ImageParticle.Scale
        }
    }

    ParticleSystem {
        id: fireballParticleSystem
        anchors.fill: parent

        Emitter {
            id: fireballEmitter
            enabled: false

            anchors.centerIn: parent
            width: 80
            height: 80

            emitRate: 500
            lifeSpan: 400
            lifeSpanVariation: 80
            size: 11
            speed: AngleDirection {
                magnitude: 60
                angleVariation: 360
            }

            Friction {
                anchors.fill: parent
                factor: 0.5
            }
        }

        ImageParticle {
            source: "../../img/effects/vehicle_fire.png"
            color: "#ffffffff"
            redVariation: 0.3
            entryEffect: ImageParticle.Scale
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