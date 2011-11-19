import QtQuick 1.1

Item {
    property int centerX: turretSize/2
    property int centerY: turretSize/2
    property string bodyTexture: ""
    property string barrelBaseTexture: ""
    property string barrelSegment1Texture: ""
    property bool firing: false
    property bool smoking: false
    property int turretSize: 60
    property int turretRotation: 0

    id: root
    width: turretSize
    height: turretSize

    transform: Rotation {
        origin.x: root.centerX; origin.y: root.centerY; angle: turretRotation
    }

    Image {
        id: body
        width: turretSize
        height: turretSize
        source: bodyTexture
        sourceSize.height: turretSize
        sourceSize.width: turretSize
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    Image {
        id: barrelBase
        width: 5
        height: 30
        anchors.bottom: body.top
        anchors.bottomMargin: -1
        anchors.horizontalCenter: body.horizontalCenter

        source: barrelBaseTexture

        Image {
            id: barrelSegment1 // Some tanks have 2 recoil segments.
            width: 3
            height: 42
            anchors.bottom: barrelBase.top
            anchors.bottomMargin: 0
            anchors.horizontalCenter: barrelBase.horizontalCenter
            z: barrelBase.z - 1

            source: barrelSegment1Texture

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
            var imgNumber = fireImage.imageNumber;

            if (imgNumber != 5) {
                fireImage.imageNumber = imgNumber + 1;
                fireImage.source = PWD + "img/effects/gun_fire" + (imgNumber + 1) + ".png";
            } else if (imgNumber == 5) {
                fireImage.imageNumber = 0;
                fireImage.source = "";
                fireTimer.stop();
            }
        }
    }

    states: [
        State {
            name: "firing"
            when: ((firing == true) || (smoking == true))
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
                            smoking = false;
                        }
                    }
                }
            }
        }
    ]
}
