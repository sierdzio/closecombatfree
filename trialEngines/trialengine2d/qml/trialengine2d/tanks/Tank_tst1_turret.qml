import QtQuick 1.1
import Qt.labs.particles 1.0

Item {
    property int centerX: body.width/2
    property int centerY: (body.height/2) + barrelBase.height + barrelSegment1.height
    property string bodyTexture: "../img/tanks/generic/tank_tst1_turret_main.png"
    property bool firing: false

    id: root
    width: body.width
    height: body.height + barrelBase.height + barrelSegment1.height

    Image {
        id: body
        width: 60
        height: 60
        source: bodyTexture
        sourceSize.height: 60
        sourceSize.width: 60
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    Rectangle {
        id: barrelBase
        width: 5
        height: 30
        color: "#7b8259"
        border.width: 1
        border.color: "#4f5e1f"
        anchors.bottom: body.top
        anchors.bottomMargin: -1
        anchors.horizontalCenter: body.horizontalCenter

        Rectangle {
            id: barrelSegment1 // Some tanks have 2 recoil segments.
            width: 3
            height: 42
            color: "#7b8259"
            border.width: 1
            border.color: "#4f5e1f"
            anchors.bottom: barrelBase.top
            anchors.bottomMargin: 0
            anchors.horizontalCenter: barrelBase.horizontalCenter
            z: barrelBase.z - 1

            Image {
                property int imageNumber: 0
                id: fireImage
                visible: true
                source: ""
                scale: 2
                anchors.bottom: barrelSegment1.top
                anchors.horizontalCenter: barrelSegment1.horizontalCenter
            }
        }
    }

    Timer {
        id: fireTimer
        interval: 20
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            if (fireImage.imageNumber == 0) {
                fireImage.imageNumber = 1;
                fireImage.source = "../img/effects/gun_fire1.png";
            } else if (fireImage.imageNumber == 1) {
                fireImage.imageNumber = 2;
                fireImage.source = "../img/effects/gun_fire2.png";
            } else if (fireImage.imageNumber == 2) {
                fireImage.imageNumber = 3;
                fireImage.source = "../img/effects/gun_fire3.png";
            } else if (fireImage.imageNumber == 3) {
                fireImage.imageNumber = 4;
                fireImage.source = "../img/effects/gun_fire4.png";
            } else if (fireImage.imageNumber == 4) {
                fireImage.imageNumber = 5;
                fireImage.source = "../img/effects/gun_fire5.png";
            } else if (fireImage.imageNumber == 5) {
                fireImage.imageNumber = 0;
                fireImage.source = "";
                fireTimer.stop();
            }
        }
    }

    states: [
        State {
            name: "firing"
            when: (firing == true)
            PropertyChanges {
                target: barrelSegment1
                anchors.bottomMargin: -25
            }
        }
    ]

    transitions: [
        Transition {
            to: "firing"
            ParallelAnimation {
                ScriptAction {
                    script: {
                        fireTimer.start();
                    }
                }
                SequentialAnimation {
                    NumberAnimation {
                        target: barrelSegment1
                        property: "anchors.bottomMargin"
                        duration: 40
                    }
                    NumberAnimation {
                        target: barrelSegment1
                        property: "anchors.bottomMargin"
                        to: 0
                        duration: 500
                        easing.type: Easing.InOutQuad
                    }
                    ScriptAction {
                        script: {
                            firing = false;
                        }
                    }
                }
            }
        }
    ]
}
