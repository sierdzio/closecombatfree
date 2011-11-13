import QtQuick 1.1
import "../tanks"

Item {
    property string mapFile: "../maps/Map_tst1.qml"

    id: root

    Tank_tst1 {
        id: tank1
        objectName: "tank1"
        x: parent.width - 150
        y: 400
    }
    Tank_tst1 {
        id: tank2
        objectName: "tank2"
        x: parent.width - 300
        y: 400
    }
    Tank_tst1 {
        id: tank3
        objectName: "tank3"
        x: parent.width - 450
        y: 400
        rotation: 180
        hullColor: "#ffffff"
    }
    Tank_tst1 {
        id: tank4
        objectName: "tank4"
        x: parent.width - 600
        y: 400
        rotation: 0
    }
    Tank_tst2 {
        id: tank5
        objectName: "tank5"
        x: parent.width - 750
        y: 400
        rotation: 0
    }
}
