import QtQuick 1.1

Item {
    property int centerX: turretBodySize/2
    property int centerY: turretBodySize/2
    property string bodyTexture: ""
    property bool firing: false
    property bool smoking: false
    property color turretColor: "#7b8259"
    property int turretBodySize: 60

    id: root
    width: turretBodySize
    height: turretBodySize

    Image {
        id: body
        width: turretBodySize
        height: turretBodySize
        source: bodyTexture
        sourceSize.height: turretBodySize
        sourceSize.width: turretBodySize
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    Rectangle {
        id: barrelBase
        width: 5
        height: 30
        color: turretColor
        border.width: 1
        border.color: "#4f5e1f"
        anchors.bottom: body.top
        anchors.bottomMargin: -1
        anchors.horizontalCenter: body.horizontalCenter

        Rectangle {
            id: barrelSegment1 // Some tanks have 2 recoil segments.
            width: 3
            height: 42
            color: turretColor
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
            var imgNumber = fireImage.imageNumber;

            if (imgNumber != 5) {
                fireImage.imageNumber = imgNumber + 1;
                fireImage.source = "../../img/effects/gun_fire" + (imgNumber + 1) + ".png";
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
