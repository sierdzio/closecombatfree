import QtQuick 1.1
import "../units/tanks/tests"

Item {
    property string mapFile: PWD + "maps/Map_tst1.qml"

    id: root
//    width: 800
//    height: 800

    Tank_tst1 {
        objectName: "tank1"
        x: 150
        y: 400
    }

    Tank_tst3 {
        objectName: "tank2"
        x: 300
        y: 400
    }

    Tank_tst1 {
        objectName: "tank3"
        x: 450
        y: 400
        rotation: 90
        hullColor: "#ffffff"
    }

    Tank_tst1 {
        objectName: "tank4"
        x: 600
        y: 400
        rotation: 0
    }

    Tank_tst2 {
        objectName: "tank5"
        x: 750
        y: 400
        rotation: 0
    }

    Tank_tst3 {
        objectName: "tank6"
        x: 750
        y: 200
    }
}