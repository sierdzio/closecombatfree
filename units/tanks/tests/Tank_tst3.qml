import QtQuick 1.1
import "../"
import "../../"

Tank {
    id: root
    unitType: "Test tank 3"
    unitLogo: PWD + "img/units/tanks/generic/tank_tst3_logo.png"
    rotationSpeed: 55
    turretRotationSpeed: 55
    turretSize: 44
    maxSpeed: 60
    acceleration: 4
    unitWidth: 50
    unitHeight: 80

    hullColor: "#505050"

    Tank_tst3_hull {
        id: hull
    }

    TurretImage {
        id: turret
        bodyTexture: PWD + "img/units/tanks/generic/tank_tst3_turret_main.png"
        barrelBaseTexture: PWD + "img/units/tanks/generic/turret_barrelBase.png"
        barrelSegment1Texture: PWD + "img/units/tanks/generic/turret_barrelSegment1_tank_tst3.png"
    }

    // This should be redesigned into a C++ property
    Item {
        id: soldierContainer

        Soldier {
            role: "Commander"
        }

        Soldier {
            role: "Gunner"
        }

        Soldier {
            role: "Loader"
        }

        Soldier {
            role: "MG gunner"
        }
    }

    Component.onCompleted: {
        var allSoldiers = soldierContainer.children;
        for (var i = 0; i < allSoldiers.length; i++) {
            addSoldier(allSoldiers[i]);
        }
    }
}
