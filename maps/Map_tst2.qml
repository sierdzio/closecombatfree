import QtQuick 1.1
import "props"

Map {
    backgroundImage: PWD + "img/maps/test2.jpg"
    hipsometricImage: PWD + "img/maps/hipsometric_test1.png"

    HouseBasic_tst {
        id: house1
        x: 20
        y: 50
    }

    Rectangle {
        id: rect2
        x: 50
        y: 260
        width: 100
        height: 100
        color: "#aabbbbcc"
    }

    Rectangle {
        id: rect3
        x: 500
        y: 260
        width: 150
        height: 300
        radius: 50
        color: "#ee1188ee"
    }
}
