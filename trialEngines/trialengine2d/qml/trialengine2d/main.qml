import QtQuick 1.1
import "tanks"
import "gui"
import "engineLogicHelpers.js" as LogicHelpers
import "engineLogicCore.js" as LogicCore

Rectangle {
    property int __aimLineRotation: 0

    id: root
    width: 1000
    height: 800
    color: "lightgray"

    Loader {
        id: contextLoader
        rotation: 0
        z: root.z + 1
    }
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
    Timer {
        id: aimLineRotationTimer // this should be changed
        interval: 120
        running: false
        repeat: true
        onTriggered: {
            if (aimLine.visible == true) {
                __aimLineRotation = LogicHelpers.rotationAngle(mouseAreaMain.mouseX,
                                                            mouseAreaMain.mouseY,
                                                            LogicCore.__handledObject.x + LogicCore.__handledObject.centerX,
                                                            LogicCore.__handledObject.y + LogicCore.__handledObject.centerY);
                aimLine.height = LogicHelpers.targetDistance(LogicCore.__handledObject.x +LogicCore. __handledObject.centerX,
                                                          LogicCore.__handledObject.y + LogicCore.__handledObject.centerY,
                                                          mouseAreaMain.mouseX,
                                                          mouseAreaMain.mouseY);
            } else {
                var tempRotation;
                tempRotation = LogicHelpers.rotationAngle(LogicCore.__handledObject.x + LogicCore.__handledObject.centerX,
                                                       LogicCore.__handledObject.y + LogicCore.__handledObject.centerY,
                                                       mouseAreaMain.mouseX,
                                                       mouseAreaMain.mouseY);
                LogicCore.__handledObject.defenceSphereRotation = LogicCore.__handledObject.rotation
                        + LogicHelpers.angleTo8Step(tempRotation);
            }
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
            if (fireImage.imageNumber == 0) {
                fireImage.imageNumber = 1;
                fireImage.source = "img/effects/gun_fire1.png";
            } else if (fireImage.imageNumber == 1) {
                fireImage.imageNumber = 2;
                fireImage.source = "img/effects/gun_fire2.png";
            } else if (fireImage.imageNumber == 2) {
                fireImage.imageNumber = 3;
                fireImage.source = "img/effects/gun_fire3.png";
            } else if (fireImage.imageNumber == 3) {
                fireImage.imageNumber = 4;
                fireImage.source = "img/effects/gun_fire4.png";
            } else if (fireImage.imageNumber == 4) {
                fireImage.imageNumber = 5;
                fireImage.source = "img/effects/gun_fire5.png";
            } else if (fireImage.imageNumber == 5) {
                fireImage.imageNumber = 0;
                fireImage.source = "";
                fireTimer.stop();
            }
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
    Tank_tst1 {
        id: tank1
        x: parent.width - 150
        y: 100
    }
    Tank_tst1 {
        id: tank2
        x: parent.width - 300
        y: 100
    }
    Tank_tst1 {
        id: tank3
        x: parent.width - 450
        y: 100
        rotation: 180
    }
    Tank_tst1 {
        id: tank4
        x: parent.width - 600
        y: 100
        rotation: 0
    }

    MouseArea {
        id: mouseAreaMain
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        z: -1

        onClicked: {
            if (mouse.button == Qt.LeftButton) {
                if (contextLoader.visible == false) {//(contextLoader.source != "") {
                    LogicCore.performContextAction(mouseAreaMain.mouseX, mouseAreaMain.mouseY);
                    return;
                } else {
                    LogicCore.cleanContextAction();
                }
            }
            else if (mouse.button == Qt.RightButton) {
                if ((LogicCore.__scheduledOperation == "Ambush") || (LogicCore.__scheduledOperation == "Defend")) {
                    LogicCore.__handledObject.defenceSphereColor = "";
                }

                LogicCore.cleanContextAction();

                // "Hide" context menu. This is suboptimal and should be changed.
                /*if (contextLoader.source != "") {
                    cleanContextAction();
                    return; // Makes app 'eat' this mouse click.
                } else*/ {
                    var child;
                    child = root.childAt(mouseAreaMain.mouseX, mouseAreaMain.mouseY);

                    if (child == mouseAreaMain) {
                        return;
                    }
                    if (child.centerX != undefined) {
                        // Fixes context menu at the centre of child object.
                        contextLoader.y = child.y + child.centerY;
                        contextLoader.x = child.x + child.centerX;

                        LogicCore.__handledObject = child;
                        // Displays the context menu. This is suboptimal.
                        contextLoader.source = "gui/ContextMenu.qml";
                        contextLoader.item.menuEntryClicked.connect(LogicCore.scheduleContextAction);
                    }
                }
            }
        }
    }
}
