import QtQuick 1.0

Rectangle {
    property int centerX: hull.width/2
    property int centerY: hull.height/2
    property int turretRotation: turret.rotation
    property string turretBodyTexture: "img/tank_tst1_turret_main.png"
    property int __tempX: x // Ugly, but I couldn't make it better, yet. Will retry later.
    property int __tempY: y // Ugly, but I couldn't make it better, yet. Will retry later.

    signal moveTo (real newX, real newY)
    onMoveTo: {
        rotation = movementRotationAngle(x, y, newX, newY);
        __tempX = newX;
        __tempY = newY;
    }

    id: root
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
        bodyTexture: turretBodyTexture

        transform: Rotation { origin.x: turret.centerX; origin.y: turret.centerY; angle: turretRotation}
    }

    Behavior on rotation {
        RotationAnimation {
            id: rotationAnimation
            property: "rotation"
            duration: 2000; direction: RotationAnimation.Shortest
            onRunningChanged: { // Not sure whether doing that will be good under all circumstances
                if (!rotationAnimation.running) {
//                    moveAnimation.start(); // Strange, but it does not work!
                    x = __tempX;
                    y = __tempY;
                }
            }
        }
    }
    Behavior on turretRotation {
        RotationAnimation {
            duration: 3000
            direction: RotationAnimation.Shortest
        }
    }
    Behavior on x {
        NumberAnimation {
            duration: 2500; easing.type: Easing.InOutQuad
        }
    }
    Behavior on y {
        NumberAnimation {
            duration: 2500; easing.type: Easing.InOutQuad
        }
    }

    function movementRotationAngle(oldX, oldY, newX, newY) {
        var result = 0;

        if (newX == oldX) {
            if (newY > oldY)
                result = 0;
            else
                result = 180;
            return result;
        }
        else if (newY == oldY) {
            if (newX > oldX)
                result = 90;
            else
                result = 270;
            return result;
        }

        var angle = (Math.atan(Math.abs(newX - oldX) / Math.abs(newY - oldY)) * 180 / Math.PI);

        if ((newY > oldY) && (newX > oldX)) // 2. quarter
            result = 180 - angle;
        else if ((newY > oldY) && (newX < oldX)) // 3. quarter
            result = 180 + angle;
        else if ((newY < oldY) && (newX < oldX)) // 4. quarter
            result = 360 - angle;
        else // 1. quarter
            result = angle;

//            console.log("Angle: " + result);
        return result;
    }
}
