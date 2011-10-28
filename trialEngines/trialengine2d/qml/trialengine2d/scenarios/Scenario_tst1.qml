import QtQuick 1.1
import "../tanks"

Item {
    Tank_tst1 {
        id: tank1
        x: parent.width - 150
        y: 400
    }
    Tank_tst1 {
        id: tank2
        x: parent.width - 300
        y: 400
    }
    Tank_tst1 {
        id: tank3
        x: parent.width - 450
        y: 400
        rotation: 180
        hullColor: "#ffffff"
    }
    Tank_tst1 {
        id: tank4
        x: parent.width - 600
        y: 400
        rotation: 0
    }
    Tank_tst2 {
        id: tank5
        x: parent.width - 750
        y: 400
        rotation: 0
        hullColor: "#f5f5f5"
    }
}
