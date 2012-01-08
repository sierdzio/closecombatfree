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
        rotation: 0
    }
    Tank_tst2 {
        objectName: "tank2"
        x: 1100
        y: 400
        rotation: 0
    }
    Tank_tst3 {
        objectName: "tank3"
        x: 450
        y: 400
        rotation: 0
    }
    Tank_tst3 {
        objectName: "tank4"
        x: 675
        y: 260
        rotation: 294.825881999401
    }
    Tank_tst2 {
        objectName: "tank5"
        x: 858.014802370574
        y: 453.518660438939
        rotation: 239.808377039042
    }

}
