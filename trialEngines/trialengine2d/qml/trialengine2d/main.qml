import QtQuick 1.0
import QtWebKit 1.0

Rectangle {
    id: main
    width: 800
    height: 600
    color: "lightgray"

    Text {
        id: textHello
        text: qsTr("Hello World")
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
    }
    Tank_tst1 {
        id: tank1
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 100
        anchors.rightMargin: 100
    }
    Tank_tst1 {
        id: tank2
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 100
        anchors.rightMargin: 250
    }
    Tank_tst1 {
        id: tank3
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 100
        anchors.rightMargin: 350
        rotation: 180
    }
    Tank_tst1 {
        id: tank4
        x: parent.width - 450
        y: 350
        rotation: 0

        property real rotationAngle: 0
        signal moveTo (real positionX, real positionY)
        onMoveTo: {
            rotationAngle = movementRotationAngle(x, y, positionX, positionY);
            moveAction.start()
        }

        function movementRotationAngle(x, y, positionX, positionY) {
            var result = 0;

            if (positionX == x) {
                if (positionY > y)
                    result = 0;
                else
                    result = 180;
                return result;
            }
            else if (positionY == y) {
                if (positionX > x)
                    result = 90;
                else
                    result = 270;
                return result;
            }

            var angle = (Math.atan(Math.abs(positionX - x) / Math.abs(positionY - y)) * 180 / Math.PI);

            if ((positionY > y) && (positionX > x)) // 2. quarter
                result = 180 - angle;
            else if ((positionY > y) && (positionX < x)) // 3. quarter
                result = 180 + angle;
            else if ((positionY < y) && (positionX < x)) // 4. quarter
                result = 360 - angle;
            else // 1. quarter
                result = angle;

            console.log("Angle: " + result);
            return result;
        }

        SequentialAnimation {
            id: moveAction
            objectName: "moveAction"
            RotationAnimation {
                target: tank4
                to: tank4.rotationAngle
                properties: "rotation"
                duration: 300; direction: RotationAnimation.Shortest
            }
            ParallelAnimation {
            NumberAnimation {
                target: tank4
                to: mouseAreaMain.mouseX
                properties: "x"; duration: 2000; easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                target: tank4
                to: mouseAreaMain.mouseY
                properties: "y"; duration: 2000; easing.type: Easing.InOutQuad
            }
            }
        }
    }

    MouseArea {
        id: mouseAreaMain
        anchors.fill: parent
        onClicked: {
            console.log("Mouse: " + mouseAreaMain.mouseX + ", " + mouseAreaMain.mouseY)
            tank4.moveTo(mouseAreaMain.mouseX, mouseAreaMain.mouseY)

            if (main.state == "")
                main.state = "afterClick"
            else
                main.state = ""
        }
    }
    MouseArea {
        id: mouseAreaTank3
        anchors.fill: tank3
        onClicked: ParallelAnimation {
            RotationAnimation {
                target: tank3
                to: 180
                properties: "turretRotation"
                duration: 3000; direction: RotationAnimation.Shortest
            }
            NumberAnimation {
                target: tank3
                to: 200
                properties: "anchors.topMargin"; duration: 2500; easing.type: Easing.InOutQuad
            }
        }
    }

    states: [
        State {
            name: "afterClick"

            PropertyChanges {
                target: textHello
                color: "#730909"
                font.bold: true
                font.pointSize: 15
            }
            PropertyChanges {
                target: tank1
                anchors.topMargin: 300
                turretRotation: 170
            }
            PropertyChanges {
                target: tank2
                anchors.topMargin: 300
                anchors.rightMargin: 500
                rotation:90
            }
        }
    ]

    transitions: Transition {
        from: ""; to: "afterClick"; reversible: true
        ParallelAnimation {
            NumberAnimation {
                target: tank1
                properties: "anchors.topMargin"; duration: 2500; easing.type: Easing.InOutQuad
            }
            RotationAnimation {
                target: tank1
                properties: "turretRotation"
                duration: 5000; direction: RotationAnimation.Shortest
            }
            SequentialAnimation {
                NumberAnimation {
                    target: tank2
                    properties: "anchors.topMargin"; duration: 2500; easing.type: Easing.InOutQuad
                }
                RotationAnimation {
                    target: tank2
                    properties: "rotation"
                    duration: 3000; direction: RotationAnimation.Shortest
                }
                NumberAnimation {
                    target: tank2
                    properties: "anchors.rightMargin"; duration: 2500; easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
