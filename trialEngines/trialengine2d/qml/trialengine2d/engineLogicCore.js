var __handledObject; // Keeps currently handled object. Bad implementation,
                    // to be fixed later. At the very least, move it into JS script.
var __scheduledOperation;

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

function rotateAimLine() {
    if (aimLine.visible == true) {
        __aimLineRotation = LogicHelpers.rotationAngle(mouseAreaMain.mouseX,
                                                    mouseAreaMain.mouseY,
                                                    __handledObject.x + __handledObject.centerX,
                                                    __handledObject.y + __handledObject.centerY);
        aimLine.height = LogicHelpers.targetDistance(__handledObject.x +  __handledObject.centerX,
                                                  __handledObject.y + __handledObject.centerY,
                                                  mouseAreaMain.mouseX,
                                                  mouseAreaMain.mouseY);
    } else {
        var tempRotation;
        tempRotation = LogicHelpers.rotationAngle(__handledObject.x + __handledObject.centerX,
                                               __handledObject.y + __handledObject.centerY,
                                               mouseAreaMain.mouseX,
                                               mouseAreaMain.mouseY);
        __handledObject.defenceSphereRotation = __handledObject.rotation
                + LogicHelpers.angleTo8Step(tempRotation);
    }
}

function handleMouseClick(mouse) {
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
            child = currentScenario.childAt(mouseAreaMain.mouseX, mouseAreaMain.mouseY);

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

function switchFireFrame(fileName) {
    var imgNumber = fireImage.imageNumber;

    if (imgNumber != 5) {
        fireImage.imageNumber = imgNumber + 1;
        fireImage.source = "img/effects/" + fileName + (imgNumber + 1) + ".png";
    } else if (imgNumber == 5) {
        fireImage.imageNumber = 0;
        fireImage.source = "";
        fireTimer.stop();
    }
}
