import QtQuick 1.0

Rectangle {
    property int centerX: hull.width/2
    property int centerY: hull.height/2
    property int turretRotation: turret.rotation

    color: "#00000000"
    width: hull.width
    height: hull.height

    Tank_tst1_hull {
        id: hull
        anchors.top: parent.top
        anchors.left: parent.left
    }

    Tank_tst1_turret {
        id: turret
        anchors.top: hull.top
        anchors.left: hull.left
        anchors.topMargin: hull.centerY - turret.centerY
        anchors.leftMargin: 1

        transform: Rotation { origin.x: turret.centerX; origin.y: turret.centerY; angle: turretRotation}
    }
}
