import QtQuick 1.1
import "tanks"
import "gui"
import "engineLogicCore.js" as LogicCore

Rectangle {
    property variant __handledObject // Keeps currently handled object. Very bad implementation,
                                     // to be fixed later. At the very least, move it into JS script.
    property string __scheduledOperation
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
                __aimLineRotation = LogicCore.rotationAngle(mouseAreaMain.mouseX,
                                                            mouseAreaMain.mouseY,
                                                            __handledObject.x + __handledObject.centerX,
                                                            __handledObject.y + __handledObject.centerY);
                aimLine.height = LogicCore.targetDistance(__handledObject.x + __handledObject.centerX,
                                                          __handledObject.y + __handledObject.centerY,
                                                          mouseAreaMain.mouseX,
                                                          mouseAreaMain.mouseY);
            } else {
                var tempRotation;
                tempRotation = LogicCore.rotationAngle(__handledObject.x + __handledObject.centerX,
                                                       __handledObject.y + __handledObject.centerY,
                                                       mouseAreaMain.mouseX,
                                                       mouseAreaMain.mouseY);
                __handledObject.defenceSphereRotation = __handledObject.rotation
                        + LogicCore.angleTo8Step(tempRotation);
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
                    performContextAction(mouseAreaMain.mouseX, mouseAreaMain.mouseY);
                    return;
                } else {
                    cleanContextAction();
                }
            }
            else if (mouse.button == Qt.RightButton) {
                if ((__scheduledOperation == "Ambush") || (__scheduledOperation == "Defend")) {
                    __handledObject.defenceSphereColor = "";
                }

                cleanContextAction();

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

                        __handledObject = child;
                        // Displays the context menu. This is suboptimal.
                        contextLoader.source = "gui/ContextMenu.qml";
                        contextLoader.item.menuEntryClicked.connect(scheduleContextAction);
                    }
                }
            }
        }
    }

    function scheduleContextAction(operation) {
        __scheduledOperation = operation;
        contextLoader.source = "";
        contextLoader.visible = false;

        // Prevents some strange errors in certain situations.
        if (__handledObject.centerY != undefined) {
            // Draw aim line for all move/attack operations.
            if ((operation != "Ambush") && (operation != "Defend")) {
                aimLine.anchors.top = __handledObject.top;
                aimLine.anchors.topMargin = __handledObject.centerY;
                aimLine.anchors.left = __handledObject.left;
                aimLine.anchors.leftMargin = __handledObject.centerX;

                if (operation == "Move fast")
                    aimLine.color = "#771b91";
                else if (operation == "Move")
                    aimLine.color = "#22ff22";
                else if (operation == "Sneak")
                    aimLine.color = "#f0dd0c";
                else if (operation == "Smoke")
                    aimLine.color = "#ffa000";
                else if (operation == "Attack")
                    aimLine.color = "#ff2222";

                aimLineRotationTimer.start();
                aimLine.visible = true;

            } else { // Draw defense 'spheres'
                if (operation == "Ambush") {
                    __handledObject.defenceSphereColor = "green";
                }
                else if (operation == "Defend") {
                    __handledObject.defenceSphereColor = "blue";
                }
                aimLineRotationTimer.start();
            }
        }
    }

    function performContextAction(targetX, targetY) {
        if ((__scheduledOperation != "Ambush") && (__scheduledOperation != "Defend")) {
            // Clear defence, if it is on.
            __handledObject.defenceSphereColor = "";

            if (__scheduledOperation == "Move") {
                __handledObject.moveTo(targetX, targetY);
            } else if (__scheduledOperation == "Attack") {
                __handledObject.fireTo(targetX, targetY);
                __handledObject.actionFinished.connect(firingActionFinished);
            }
        }

        cleanContextAction();
    }

    function firingActionFinished(targetX, targetY) {
        // A good place to include terrain recognition
        // for landing shells
        fireImage.x = targetX;
        fireImage.y = targetY;
        fireTimer.start();
    }

    function cleanContextAction() {
        aimLineRotationTimer.stop();
        aimLine.visible = false;
        aimLine.height = 150;
        contextLoader.source = "";
        contextLoader.visible = true;
        __scheduledOperation = "";
        __handledObject = 0;
    }
}
