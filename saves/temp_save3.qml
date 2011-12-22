import QtQuick 1.1
import "qrc:/units/tanks/tests"
import "qrc:/core/units"


Item {
    property string mapFile: "qrc:/maps/Map_tst3.qml"

    id: root

    Tank_tst1 {
        objectName: "tank1"
        x: 331
        y: 674
        rotation: 218.634298113517
        turretRotation: 0
        hullColor: "#7b8259"
    }
    Tank_tst2 {
        objectName: "tank2"
        x: 1100
        y: 400
        rotation: 0
        turretRotation: 64
        hullColor: "#505050"
    }
    Tank_tst3 {
        objectName: "tank3"
        x: 237
        y: 684
        rotation: 216.869897645844
        turretRotation: 0
        hullColor: "#ffeebb"
    }
    Tank_tst3 {
        objectName: "tank4"
        x: 537
        y: 684
        rotation: 216.869897645844
        turretRotation: 0
        hullColor: "#505050"
    }
    Tank_tst2 {
        objectName: "tank5"
        x: 950
        y: 400
        rotation: 0
        turretRotation: 91
        hullColor: "#505050"
    }

}
