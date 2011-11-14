var handledObject; // Keeps currently handled object. Bad implementation,
                    // to be fixed later. At the very least, move it into JS script.
var scheduledOperation;

function scheduleContextAction(operation) {
    scheduledOperation = operation;
    contextLoader.source = "";
    contextLoader.visible = false;

    // Prevents some strange errors in certain situations.
    if (handledObject.centerY != undefined) {
        // Draw aim line for all move/attack operations.
        if ((operation != "Ambush") && (operation != "Defend")) {
            aimLine.x = handledObject.x + handledObject.centerX;
            aimLine.y = handledObject.y + handledObject.centerY;

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
                handledObject.defenceSphereColor = "green";
                handledObject.changeStatus("AMBUSHING");
            }
            else if (operation == "Defend") {
                handledObject.defenceSphereColor = "blue";
                handledObject.changeStatus("DEFENDING");
            }
            aimLineRotationTimer.start();
        }
    }
}

function performContextAction(targetX, targetY) {
    if ((scheduledOperation != "Ambush") && (scheduledOperation != "Defend")) {
        // Clear defence, if it is on.
        handledObject.defenceSphereColor = "";
        handledObject.changeStatus("READY");

        if (scheduledOperation == "Move") {
            handledObject.moveTo(targetX, targetY);
        } else if (scheduledOperation == "Move fast") {
            handledObject.moveFastTo(targetX, targetY);
        } else if (scheduledOperation == "Sneak") {
            handledObject.sneakTo(targetX, targetY);
        } else if (scheduledOperation == "Attack") {
            handledObject.fireTo(targetX, targetY);
            handledObject.actionFinished.connect(firingActionFinished);
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
    aimLine.height = 5;
    contextLoader.source = "";
    contextLoader.visible = true;
    scheduledOperation = "";
    handledObject = 0;
}

function rotateAimLine() {
    if (aimLine.visible == true) {
        aimLine.x = handledObject.x + handledObject.centerX;
        aimLine.y = handledObject.y + handledObject.centerY;

        __aimLineRotation = LogicHelpers.rotationAngle(mouseAreaMain.mouseX,
                                                    mouseAreaMain.mouseY,
                                                    handledObject.x + handledObject.centerX,
                                                    handledObject.y + handledObject.centerY);
        aimLine.height = LogicHelpers.targetDistance(handledObject.x +  handledObject.centerX,
                                                  handledObject.y + handledObject.centerY,
                                                  mouseAreaMain.mouseX,
                                                  mouseAreaMain.mouseY);
    } else {
        var tempRotation;
        tempRotation = LogicHelpers.rotationAngle(handledObject.x + handledObject.centerX,
                                               handledObject.y + handledObject.centerY,
                                               mouseAreaMain.mouseX,
                                               mouseAreaMain.mouseY);
        handledObject.defenceSphereRotation = handledObject.rotation
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
        if ((scheduledOperation == "Ambush") || (scheduledOperation == "Defend")) {
            handledObject.defenceSphereColor = "";
        }
        cleanContextAction();

        var child;
        child = childAt(mouseAreaMain.mouseX, mouseAreaMain.mouseY);

        if (child == mouseAreaMain) {
            return;
        }
        if (child.centerX != undefined) {
            // Fixes context menu at the centre of child object.
            setContextMenuPosition(contextLoader,
                                   child.x + child.centerX,
                                   child.y + child.centerY);
//            contextLoader.y = child.y + child.centerY;
//            contextLoader.x = child.x + child.centerX;

            handledObject = child;
            // Displays the context menu. This is suboptimal.
            contextLoader.source = "../gui/ContextMenu.qml";
            contextLoader.item.menuEntryClicked.connect(scheduleContextAction);
        }
    }
}

function handleMouseClickRoster(mouse) {
    if (mouse.button == Qt.LeftButton) {
        if (contextLoader.visible == false) {//(contextLoader.source != "") {
//            performContextAction(mouseAreaMain.mouseX, mouseAreaMain.mouseY);
//            return;
        } else {
            cleanContextAction();
        }
    }
    else if (mouse.button == Qt.RightButton) {
        if ((scheduledOperation == "Ambush") || (scheduledOperation == "Defend")) {
            handledObject.defenceSphereColor = "";
        }
        cleanContextAction();

        var child;
        child = roster.childAt(mouseAreaRoster.mouseX, mouseAreaRoster.mouseY);
        var unit;
        unit = roster.getUnitAt(mouseAreaRoster.mouseX, mouseAreaRoster.mouseY);

        if (unit.centerX != undefined) {
            // Fixes context menu at the centre of child object.
            setContextMenuPosition(contextLoader,
                                   roster.x + child.x + (roster.entryWidth/2),
                                   roster.y + child.y + (roster.entryHeight/2));

            handledObject = unit;
            // Displays the context menu. This is suboptimal.
            contextLoader.source = "../gui/ContextMenu.qml";
            contextLoader.item.menuEntryClicked.connect(scheduleContextAction);
        }
    }
}

function switchFireFrame(fileName) {
    var imgNumber = fireImage.imageNumber;

    if (imgNumber != 5) {
        fireImage.imageNumber = imgNumber + 1;
        fireImage.source = "../img/effects/" + fileName + (imgNumber + 1) + ".png";
    } else if (imgNumber == 5) {
        fireImage.imageNumber = 0;
        fireImage.source = "";
        fireTimer.stop();
    }
}

function setContextMenuPosition(menu, x, y) {
    if ((x + menu.width) > root.width)
        menu.x = root.width - menu.width;
    else
        menu.x = x;

    if ((y + menu.height) > root.height)
        menu.y = root.height - menu.height;
    else
        menu.y = y;
}
