import QtQuick 1.1
import "qrc:/units/tanks/tests"
import "qrc:/core/units"

Item {
    property string mapFile: "qrc:/maps/Map_tst1.qml"

    id: root

    Tank_tst1 {
        objectName: "tank1"
        x: 150
        y: 400
    }

    Tank_tst2 {
        objectName: "tank2"
        x: 150
        y: 900
        unitSide: "bad"
    }

    Tank_tst3 {
        objectName: "tank3"
        x: 350
        y: 400
        hullColor: "#ffeebb"
    }

    Tank_tst3 {
        objectName: "tank4"
        x: 350
        y: 900
        rotation: 0
        unitSide: "bad"
    }

    Tank_tst2 {
        objectName: "tank5"
        x: 1100
        y: 900
        rotation: 0
        unitSide: "good"
    }

    Tank_tst2 {
        objectName: "tank6"
        x: 500
        y: 400
    }
}
