import QtQuick 1.1
import "../units/tanks/tests"
import "../qml/units"

Item {
    property string mapFile: "../maps/Map_tst3.qml"

    id: root

    Tank_tst1 {
        objectName: "tank1"
        x: 550
        y: 400
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
    }

    Tank_tst2 {
        objectName: "tank5"
        x: 950
        y: 400
        rotation: 0
    }
}
