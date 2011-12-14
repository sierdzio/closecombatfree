import QtQuick 1.1
import "qrc:/core/maps"
import "props"

Map {
    backgroundImage: "qrc:/img/maps/test3.jpg"
    hipsometricImage: "qrc:/img/maps/hipsometric_test1.png"

    HouseBasic_tst {
        id: house1
        x: 20
        y: 50
    }

    Tree_tst {
        x: 220
        y: 30
    }

    Tree_tst {
        x: 250
        y: 30
    }

    Tree_tst {
        x: 280
        y: 30
    }

    Rectangle {
        objectName: "Gray rectangle"
        id: rect2
        x: 360
        y: 50
        width: 100
        height: 100
        color: "#aabbbbcc"
        opacity: propOpacity
    }

    Rectangle {
        objectName: "Shallow water"
        id: rect3
        x: 905
        y: 162
        width: 150
        height: 300
        radius: 50
        color: "#ee1188ee"
        opacity: propOpacity
    }
}
