import QtQuick 1.1
import "qrc:/units/tanks/tests"
import "qrc:/core/units"


Item {
    property string mapFile: "qrc:/maps/Map_tst2.qml"

    id: root

    Tank_tst1 {
        objectName: "tank1"
        x: 150
        y: 400
        rotation: 0
    }
    Tank_tst3 {
        objectName: "tank2"
        x: 300
        y: 400
        rotation: 0
    }
    Tank_tst1 {
        objectName: "tank3"
        x: 450
        y: 400
        rotation: 90
    }
    Tank_tst1 {
        objectName: "tank4"
        x: 596
        y: 270
        rotation: 358.23760897634
    }
    Tank_tst2 {
        objectName: "tank5"
        x: 752
        y: 280
        rotation: 0.954841253872189
    }
    Tank_tst3 {
        objectName: "tank6"
        x: 617
        y: 69
        rotation: 314.565949367861
    }

}
