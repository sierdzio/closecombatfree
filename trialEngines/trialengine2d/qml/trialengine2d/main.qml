import QtQuick 1.1
import "tanks"
import "gui"
import "scenarios"
import "maps"
import "engineLogicHelpers.js" as LogicHelpers
import "engineLogicCore.js" as LogicCore

Rectangle {
    property int __aimLineRotation: 0

    id: root
    width: 800
    height: 800
    color: "lightgray"

    Scenario { // that, or create a Scenario main object, and use it accordingly.
        id: currentScenario
        anchors.fill: parent

        Rectangle {
            id: aimLine
            z: root.z + 1
            visible: false
            width: 3
            height: 150

            transform: Rotation {
                origin.x: 1
                origin.y: 0
                angle: __aimLineRotation
            }
        }
    }

    Loader {
        id: contextLoader
        rotation: 0
        z: root.z + 1
    }
    Timer {
        id: aimLineRotationTimer // this should be changed
        interval: 120
        running: false
        repeat: true
        onTriggered: {
            LogicCore.rotateAimLine();
        }
    }

    Image {
        property int imageNumber: 0
        id: fireImage
        visible: true
        source: ""
        scale: 3
        z: 5
    }
    Timer {
        id: fireTimer
        interval: 80
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            LogicCore.switchFireFrame("gun_fire");
        }
    }

    Text {
        id: textHello
        text: qsTr("Hello World")
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
    }
// tanks were here

    MouseArea {
        id: mouseAreaMain
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        z: -1

        onClicked: {
            LogicCore.handleMouseClick(mouse);
        }
    }
}
