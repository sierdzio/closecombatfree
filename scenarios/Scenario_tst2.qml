import QtQuick 1.1
import "../units/tanks/tests"
import "../qml/units"

Item {
    property string mapFile: "../maps/Map_tst1.qml"

    id: root

    Tank_tst1 {
        objectName: "tank1"
        x: 550
        y: 400

        Component.onCompleted: {
            fireTo(650, 300);
            smokeTo(700, 250);
            fireTo(750, 200);
        }
    }

    Tank_tst2 {
        objectName: "tank2"
        x: 1100
        y: 400
    }

    Tank_tst3 {
        objectName: "tank3"
        x: 450
        y: 400
        hullColor: "#ffeebb"
    }

    Tank_tst3 {
        objectName: "tank4"
        x: 750
        y: 400
        rotation: 0

        Component.onCompleted: {
            queueOrder ("Move", 700, 300);
            queueOrder ("Move", 500, 250);
            queueOrder ("Attack", 50, 50);
        }
    }

    Tank_tst2 {
        objectName: "tank5"
        x: 950
        y: 400
        rotation: 0

        Component.onCompleted: {
            queueOrder ("Move", 700, 600);
            queueOrder ("Move fast", 500, 650);
            queueOrder ("Attack", 50, 50);
        }
    }
}
