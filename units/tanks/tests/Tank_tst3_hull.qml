import QtQuick 1.1
import "../"

Hull {
    Image {
        id: hull
        width: hullWidth
        height: hullHeight
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        source: PWD + "img/units/tanks/generic/tank_tst3_hull.png"
    }
}
