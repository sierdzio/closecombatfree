var effectsContainer = new Array();

function scheduleContextAction(index, operation) {
    var child = units.item.children[index];
    child.scheduledOperation = operation;
    contextLoader.source = "";
    contextLoader.visible = false;

    // Prevents some strange errors in certain situations.
    if (child.centerY != undefined) {
        // Draw aim line for all move/attack operations.
        if ((operation != "Ambush") && (operation != "Defend")) {
            aimLine.x = child.x + child.centerX;
            aimLine.y = child.y + child.centerY;

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

            rotationTimer.start();
            aimLine.visible = true;

        } else { // Draw defense 'spheres'
            if (operation == "Ambush") {
                child.defenceSphereColor = "green";
                child.changeStatus("AMBUSHING");
            }
            else if (operation == "Defend") {
                child.defenceSphereColor = "blue";
                child.changeStatus("DEFENDING");
            }
            rotationTimer.start();
        }
    }
}

function performContextAction(index, targetX, targetY) {
    var child = units.item.children[index];
    var scheduledOperation = child.scheduledOperation;

    if ((scheduledOperation != "Ambush") && (scheduledOperation != "Defend")) {
        // Clear defence, if it is on.
        child.defenceSphereColor = "";
        child.changeStatus("READY");

        if (scheduledOperation == "Move") {
            child.moveTo(targetX, targetY);
        } else if (scheduledOperation == "Move fast") {
            child.moveFastTo(targetX, targetY);
        } else if (scheduledOperation == "Sneak") {
            child.sneakTo(targetX, targetY);
        } else if (scheduledOperation == "Attack") {
            child.fireTo(targetX, targetY);
            child.actionFinished.connect(firingActionFinished);
        } else if (scheduledOperation == "Smoke") {
            child.smokeTo(targetX, targetY);
            child.actionFinished.connect(firingActionFinished);
        }
    }

    cleanContextAction();
}

function firingActionFinished(index, targetX, targetY) {
    // This component renders in=game effects (not all,
    // but for example muzzle flashes, explosions etc.)
    var component = Qt.createComponent("Effect.qml");

    // A good place to include terrain recognition
    // for landing shells
    var unit = units.item.children[index];
    var effectIndex;

    if (component.status == Component.Ready) {
        var effect = component.createObject(effectsItemContainer);
    }

    effectsContainer.push(effect);
    effectIndex = effectsContainer.length - 1;

    if (unit.scheduledOperation == "Attack") {
        effectsContainer[effectIndex].animationString = "gun_fire";
    }
    else if(unit.scheduledOperation == "Smoke") {
        effectsContainer[effectIndex].animationString = "smoke_fire";
    }

    effectsContainer[effectIndex].x = targetX;
    effectsContainer[effectIndex].y = targetY;
    effectsContainer[effectIndex].running = true;

    if (effectsTimer.running == false)
        effectsTimer.start();
}


function updateEffects() {
    var haveAllEffectsFinished = true;
    for (var i = 0; i < effectsContainer.length; i++) {
        if (effectsContainer[i].running == true) {
            switchFireFrame(i);
            haveAllEffectsFinished = false;
        }
    }

    // Clear list if all effects have finished.
    // Crude, but might optimise the code a bit.
    if (haveAllEffectsFinished == true) {
        for (var i = 0; i <= effectsContainer.length; i++) {
            effectsContainer.pop();
        }
        effectsTimer.stop();
    }

//    console.log("Running animations: " + effectsContainer.length
//                + ". Timer running: " + effectsTimer.running);
}

function switchFireFrame(effectIndex) {
    var i = effectIndex;
    var imgNumber = effectsContainer[i].imageNumber;

    if (imgNumber != 5) {
        effectsContainer[i].imageNumber = imgNumber + 1;
        effectsContainer[i].source = "../img/effects/" + effectsContainer[i].animationString + (imgNumber + 1) + ".png";
    } else if (imgNumber == 5) {
        effectsContainer[i].imageNumber = 0;
        effectsContainer[i].source = "";
        effectsContainer[i].running = false;
    }
}

function cleanContextAction() {
    rotationTimer.stop();
    aimLine.visible = false;
    aimLine.height = 5;
    __unitIndex = -1;
    contextLoader.source = "";
    contextLoader.visible = true;
}

function rotateAimLine() {
    var child = units.item.children[__unitIndex];

    if (aimLine.visible == true) {
        aimLine.x = child.x + child.centerX;
        aimLine.y = child.y + child.centerY;

        __aimLineRotation = LogicHelpers.rotationAngle(mouseAreaMain.mouseX,
                                                    mouseAreaMain.mouseY,
                                                    child.x + child.centerX,
                                                    child.y + child.centerY);
        aimLine.height = LogicHelpers.targetDistance(child.x +  child.centerX,
                                                  child.y + child.centerY,
                                                  mouseAreaMain.mouseX,
                                                  mouseAreaMain.mouseY);
    } else {
        var tempRotation;
        tempRotation = LogicHelpers.rotationAngle(child.x + child.centerX,
                                               child.y + child.centerY,
                                               mouseAreaMain.mouseX,
                                               mouseAreaMain.mouseY);
        child.defenceSphereRotation = child.rotation
                + LogicHelpers.angleTo8Step(tempRotation);
    }
}

function handleMouseClick(mouse) {
    if (mouse.button == Qt.LeftButton) {
        if (contextLoader.visible == false) {
            performContextAction(__unitIndex, mouseAreaMain.mouseX, mouseAreaMain.mouseY);
            return;
        } else {
            cleanContextAction();
        }
    }
    else if (mouse.button == Qt.RightButton) {
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

            __unitIndex = childIndex(child);
            // Displays the context menu. This is suboptimal.
            contextLoader.source = "../gui/ContextMenu.qml";
            contextLoader.item.unitIndex = __unitIndex;
            contextLoader.item.menuEntryClicked.connect(scheduleContextAction);
        }
    }
}

function handleMouseClickRoster(mouse) {
    if (mouse.button == Qt.LeftButton) {
        if (contextLoader.visible == false) {
//            performContextAction(mouseAreaMain.mouseX, mouseAreaMain.mouseY);
//            return;
        } else {
            cleanContextAction();
        }
    }
    else if (mouse.button == Qt.RightButton) {
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

            __unitIndex = childIndex(unit);
            // Displays the context menu. This is suboptimal.
            contextLoader.source = "../gui/ContextMenu.qml";
            contextLoader.item.unitIndex = __unitIndex;
            contextLoader.item.menuEntryClicked.connect(scheduleContextAction);
        }
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
