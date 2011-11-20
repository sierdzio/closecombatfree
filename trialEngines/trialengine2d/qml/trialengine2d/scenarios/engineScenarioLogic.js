var effectsContainer = new Array();
var orderMarkersContainer = new Array();
var unitGroups = new Array(10);

function scheduleContextAction(index, operation) {
    var children;
    var child;

    children = selectedUnits();
    child = units.item.children[index];

    child.scheduledOperation = operation;
    contextLoader.source = "";
    contextLoader.visible = false;

    // Prevents some strange errors in certain situations.
    if (child.centerY != undefined) {
        // Draw aim line for all move/attack operations.
        if ((operation != "Ambush") && (operation != "Defend")) {
            aimLine.x = child.x + child.centerX;
            aimLine.y = child.y + child.centerY;

            aimLine.color = LogicHelpers.colorForOrder(operation);
            rotationTimer.start();
            aimLine.visible = true;

        } else { // Draw defense 'spheres'
            if (operation == "Ambush") {
                for (var i = 0; i < children.length; i++) {
                    children[i].defenceSphereColor = "green";
                    children[i].changeStatus("AMBUSHING");
                }
            }
            else if (operation == "Defend") {
                for (var i = 0; i < children.length; i++) {
                    children[i].defenceSphereColor = "blue";
                    children[i].changeStatus("DEFENDING");
                }
            }
            rotationTimer.start();
        }
    }
}

function performContextAction(index, targetX, targetY) {
    var children = selectedUnits();
    var child = units.item.children[index];//children[0];
    var scheduledOperation = child.scheduledOperation;

    var tempX = 0;
    var tempY = 0;
    if ((scheduledOperation != "Ambush") && (scheduledOperation != "Defend")) {
        for (var i = 0; i < children.length; i++) {
            child = children[i];
            child.scheduledOperation = scheduledOperation;

            if ((child.unitIndex != index) && (i >= 1)) {
                tempX = targetX + (child.x - children[i - 1].x);
                tempY = targetY + (child.y - children[i - 1].y);
//                console.log("Child X: " + child.x + ", last child X: " + children[i - 1].x);
            } else if ((child.unitIndex != index)) {
                tempX = targetX + (child.x - children[i + 1].x);
                tempY = targetY + (child.y - children[i + 1].y);
//                console.log("Child X: " + child.x + ", last child X: " + children[i + 1].x);
            } else {
                tempX = targetX;
                tempY = targetY;
            }

            // Clear defence, if it is on.
            child.defenceSphereColor = "";
            child.changeStatus("READY");

            if (scheduledOperation == "Move") {
                child.moveTo(tempX, tempY);
            } else if (scheduledOperation == "Move fast") {
                child.moveFastTo(tempX, tempY);
            } else if (scheduledOperation == "Sneak") {
                child.sneakTo(tempX, tempY);
            } else if (scheduledOperation == "Attack") {
                child.fireTo(tempX, tempY);
            } else if (scheduledOperation == "Smoke") {
                child.smokeTo(tempX, tempY);
            }
            child.actionFinished.connect(actionFinished);

            setOrderMarker(child.unitIndex, scheduledOperation, tempX, tempY);
        }
    }
    cleanContextAction();
}

function actionFinished(index, targetX, targetY) {
    var scheduledOperation = units.item.children[index].scheduledOperation;
    if ((scheduledOperation != "Move")
            && (scheduledOperation != "Move fast")
            && (scheduledOperation != "Sneak")) {
        firingActionFinished(index, targetX, targetY);
    }

    calculateOrderMarkerVisibility(index);
}

function firingActionFinished(index, targetX, targetY) {
    // This component renders in-game effects (not all,
    // but for example muzzle flashes, explosions etc.)
    var component = Qt.createComponent("Effect.qml");

    // A good place to include terrain recognition
    // for landing shells
    var unit = units.item.children[index];
//    var effectIndex;
    var effect;

    if (component.status == Component.Ready) {
        effect = component.createObject(itemContainer);
    }

    effectsContainer.push(effect);
//    effectIndex = effectsContainer.length - 1;

    if (unit.scheduledOperation == "Attack") {
        effect.animationString = "gun_fire";
    }
    else if(unit.scheduledOperation == "Smoke") {
        effect.animationString = "smoke_fire";
    }

    effect.x = targetX;
    effect.y = targetY;
    effect.running = true;

    if (effectsTimer.running == false)
        effectsTimer.start();
}


function updateEffects() {
    var haveAllEffectsFinished = true;
    for (var i = 0; i < effectsContainer.length; i++) {
        if (effectsContainer[i].running == true) {
            switchEffectFrame(i);
            haveAllEffectsFinished = false;
        }
    }

    // Clear list if all effects have finished.
    // Crude, but might optimise the code a bit.
    if (haveAllEffectsFinished == true) {
        for (var i = 0; i <= effectsContainer.length; i++) {
            var effect = effectsContainer.pop();
            effect.destroy();
        }
        effectsTimer.stop();
    }

//    console.log("Running animations: " + effectsContainer.length
//                + ". Timer running: " + effectsTimer.running);
}

function switchEffectFrame(effectIndex) {
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

function updateAimLine() {
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
            selectUnitFromGameArea(mouse);
        }
    }
    else if (mouse.button == Qt.RightButton) {
        cleanContextAction();

        var child;
        child = childAt(mouseAreaMain.mouseX, mouseAreaMain.mouseY);

        if (child == mouseAreaMain || child == null) {
            deselectAllUnits();
            return;
        }
        if (child.centerX != undefined) {
            if (child.selected == false) {
                selectUnitFromGameArea(mouse);
            }

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
            selectUnitFromRoster(mouse);
        }
    }
    else if (mouse.button == Qt.RightButton) {
        cleanContextAction();

        var child;
        child = roster.childAt(mouse.x, mouse.y);
        var unit;
        unit = roster.getUnitAt(mouse.x, mouse.y);

        if (unit.centerX != undefined) {
            if (unit.selected == false) {
                selectUnitFromRoster(mouse);
            }
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

function handlePressAndHold(mouse) {
    rubberBand.x = mouse.x;
    rubberBand.y = mouse.y;
    rubberBand.height = 2;
    rubberBand.width = 2;
    rubberBand.visible = true;

    // Saves the state of modifiers.
//    rubberBandTimer.__modifiers = mouse.modifiers;

    if (mouse.modifiers == Qt.NoModifier)
        deselectAllUnits();

    if (rubberBandTimer.running == false)
        rubberBandTimer.start();
}

function handleMouseReleased() {
    if (rubberBandTimer.running == true) {
        rubberBandTimer.stop();
        rubberBand.visible = false;
        rubberBand.height = 2;
        rubberBand.width = 2;
    }
}

function updateRubberBand(x, y) {
    var rubberX, rubberY, rubberX2, rubberY2; // 2 edges of the rubber band,
                                              // in root's coordinates.

    // Adjusting rubber band's shape:
    if ((x > rubberBand.x) && (y > rubberBand.y)) {
        // Bottom-right quarter
        __rubberBandRotation = 0;
        rubberBand.width = x - rubberBand.x;
        rubberBand.height = y - rubberBand.y;

        rubberX = rubberBand.x;
        rubberY = rubberBand.y;
        rubberX2 = rubberBand.x + rubberBand.width;
        rubberY2 = rubberBand.y + rubberBand.height;
    } else if ((x > rubberBand.x) && (y < rubberBand.y)) {
        // Top-right quarter
        __rubberBandRotation = 270;
        rubberBand.width = rubberBand.y - y;
        rubberBand.height = x - rubberBand.x;

        rubberX = rubberBand.x;
        rubberY = rubberBand.y - rubberBand.width;
        rubberX2 = rubberBand.x + rubberBand.height;
        rubberY2 = rubberBand.y;
    } else if ((x < rubberBand.x) && (y > rubberBand.y)) {
        // Bottom-left quarter
        __rubberBandRotation = 90;
        rubberBand.width = y - rubberBand.y;
        rubberBand.height = rubberBand.x - x;

        rubberX = rubberBand.x - rubberBand.height;
        rubberY = rubberBand.y;
        rubberX2 = rubberBand.x;
        rubberY2 = rubberBand.y + rubberBand.width;
    } else if ((x < rubberBand.x) && (y < rubberBand.y)) {
        // Top-left quarter
        __rubberBandRotation = 180;
        rubberBand.width = rubberBand.x - x;
        rubberBand.height = rubberBand.y - y;

        rubberX = rubberBand.x - rubberBand.width;
        rubberY = rubberBand.y - rubberBand.height;
        rubberX2 = rubberBand.x;
        rubberY2 = rubberBand.y;
    } else if ((x == rubberBand.x) || (y == rubberBand.y)) {
        rubberBand.height = 2;
        rubberBand.width = 2;

        rubberX = rubberBand.x;
        rubberY = rubberBand.y;
        rubberX2 = rubberBand.x;
        rubberY2 = rubberBand.y;
    }

//    test1.x = rubberX;
//    test1.y = rubberY;
//    test2.x = rubberX2;
//    test2.y = rubberY2;

    // Selecting units:
    var children = units.item.children;
    for (var i = 0; i < children.length; i++) {
        var child = children[i];

        if (child.selected == true)
            continue;

        if ((child.x <= rubberX2) && (child.x >= rubberX)
                && (child.y <= rubberY2) && (child.y >= rubberY)) {
            selectUnit(child.unitIndex, Qt.ControlModifier);
        }
    }
}

function selectUnitFromGameArea(mouse) {
    var child = childAt(mouse.x, mouse.y);

    if (child == null) {
        deselectAllUnits();
        return;
    }

    if (child.unitStatus != undefined) {
        selectUnit(child.unitIndex, mouse.modifiers);
    } else {
        deselectAllUnits();
    }
}

function selectUnitFromRoster(mouse) {
    var child = roster.getUnitAt(mouse.x, mouse.y);

    if (child != 0) {
        selectUnit(child.unitIndex, mouse.modifiers);
    }
}

function selectUnit(index, modifier) {
    if (modifier == Qt.NoModifier) {
        deselectAllUnits();
        units.item.children[index].selected = true;
    } else if (modifier == Qt.ControlModifier) {
        if (units.item.children[index].selected == true)
            units.item.children[index].selected = false;
        else if (units.item.children[index].selected == false)
            units.item.children[index].selected = true;
    }

    calculateOrderMarkerVisibility(index);
}

function deselectUnit(index) {
    units.item.children[index].selected = false;
    calculateOrderMarkerVisibility(index);
}

function deselectAllUnits() {
    var children = units.item.children;
    for (var i = 0; i < children.length; i++) {
        deselectUnit(i);
    }
}

function selectedUnitsCount() {
    var result = 0;
    var children = units.item.children;
    for (var i = 0; i < children.length; i++) {
        if (children[i].selected == true)
            result ++;
    }
    return result;
}

function selectedUnits() {
    var result = new Array();
    var children = units.item.children;
    for (var i = 0; i < children.length; i++) {
        if (children[i].selected == true)
            result.push(children[i]);
    }
    return result;
}

function groupUnits(groupNumber) {
    if (selectedUnitsCount() == 0) {
        return;
    }

    // Remove old members.
    if (unitGroups[groupNumber] != undefined) {
        for (var i = 0; i < unitGroups[groupNumber].length; i++) {
            if (unitGroups[groupNumber][i].selected == false)
                unitGroups[groupNumber][i].groupNumber = 0;
        }
    }

    var group = selectedUnits();
    unitGroups[groupNumber] = group;

    for (var i = 0; i < group.length; i++) {
        group[i].groupNumber = groupNumber;
    }
    console.log("Group " + groupNumber + " created.");
}

function selectGroup(groupNumber) {
    var group = unitGroups[groupNumber];

    if (group != undefined) {
        deselectAllUnits();
    } else {
        return;
    }

    for (var i = 0; i < group.length; i++) {
        group[i].selected = true;
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

function digitPressed(event) {
    var result = -1;

    if (event.key == Qt.Key_1)
        result = 1;
    else if (event.key == Qt.Key_2)
        result = 2;
    else if (event.key == Qt.Key_3)
        result = 3;
    else if (event.key == Qt.Key_4)
        result = 4;
    else if (event.key == Qt.Key_5)
        result = 5;
    else if (event.key == Qt.Key_6)
        result = 6;
    else if (event.key == Qt.Key_7)
        result = 7;
    else if (event.key == Qt.Key_8)
        result = 8;
    else if (event.key == Qt.Key_9)
        result = 9;
    else if (event.key == Qt.Key_0)
        result = 10;

    return result;
}

function calculateOrderMarkerVisibility(index) {
    var orderMarker = orderMarkersContainer[index];
    var child = units.item.children[index];

    if (child.selected == true) {
        if ((child.unitStatus == "READY")
                || (child.unitStatus == "DEFENDING")
                || (child.unitStatus == "AMBUSHING")) {
            orderMarker.visible = false;
        } else {
            orderMarker.visible = true;
        }
    } else {
        orderMarker.visible = false;
    }
}

function setOrderMarker(index, orderName, targetX, targetY) {
    var orderMarker = orderMarkersContainer[index];
    orderMarker.x = targetX - orderMarker.centerX;
    orderMarker.y = targetY - orderMarker.centerY;
    orderMarker.orderColor = LogicHelpers.colorForOrder(orderName);
    orderMarker.visible = true;
}

function createOrderMarkers() {
    for (var i = 0; i < units.item.children.length; i++) {
        // This component renders an order marker.
        var component = Qt.createComponent(PWD + "gui/OrderMarker.qml");
        var marker;

        if (component.status == Component.Ready) {
            marker = component.createObject(itemContainer);
        }

        marker.visible = false;
        orderMarkersContainer.push(marker);
    }
}
