import QtQuick 2.1
import "../units/tanks/tests"
import "../qml/units"


Item {
    property string mapFile: "maps/Map_tst1.qml"

    id: root

    Tank_tst1 {
        objectName: "tank1"
        x: 151
        y: 683
        rotation: 179.797542257782
        unitSide: "neutral"
        turretRotation: 0
        hullColor: "#7b8259"
    }
    Tank_tst2 {
        objectName: "tank2"
        x: 150
        y: 900
        rotation: 0
        unitSide: "bad"
        turretRotation: 0
        hullColor: "#505050"
    }
    Tank_tst3 {
        objectName: "tank3"
        x: 357
        y: 693
        rotation: 178.631419193047
        unitSide: "neutral"
        turretRotation: 0
        hullColor: "#ffeebb"
    }
    Tank_tst3 {
        objectName: "tank4"
        x: 350
        y: 900
        rotation: 0
        unitSide: "bad"
        turretRotation: 0
        hullColor: "#505050"
    }
    Tank_tst2 {
        objectName: "tank5"
        x: 1100
        y: 900
        rotation: 0
        unitSide: "good"
        turretRotation: 0
        hullColor: "#505050"
    }
    Tank_tst2 {
        objectName: "tank6"
        x: 944
        y: 553
        rotation: 109.01367438853
        unitSide: "neutral"
        turretRotation: 47
        hullColor: "#505050"
    }

}
