import QtQuick 1.1
import "../"
import "../../"

Tank {
    id: root
    unitType: "Test tank 1"
    unitLogo: PWD + "img/units/tanks/generic/tank_tst1_logo.png"
    rotationSpeed: 40
    turretRotationSpeed: 35
    maxSpeed: 20
    acceleration: 2
    unitWidth: 62
    unitHeight: 100

    Tank_tst1_hull {
        id: hull
    }

    Tank_tst1_turret {
        id: turret
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
            role: "Assistant"
        }

        Soldier {
            role: "Assistant"
        }
    }

    Component.onCompleted: {
        var allSoldiers = soldierContainer.children;
        for (var i = 0; i < allSoldiers.length; i++) {
            addSoldier(allSoldiers[i]);
        }
    }
}
