var effectsContainer = new Array();
var orderMarkersContainer = new Array();
var unitGroups = new Array(10);

function scheduleContextAction(index, operation) {
    var children;
    var child;

    children = selectedUnits();
    if (units.item.children[index] != undefined) {
        child = units.item.children[index];
    } else {
        child = children[0];
        __unitIndex = child.unitIndex;
    }

    child.scheduledOperation = operation;
    contextLoader.source = "";
    contextLoader.visible = false;

    // Prevents some strange errors in certain situations.
    if (child.centerY != undefined) {
        if (operation == "Stop") {
            // Iterate over every unit!
            for (var i = 0; i < children.length; i++) {
                children[i].cancelOrder();
                calculateOrderMarkerVisibility(children[i].unitIndex);
            }
            cleanContextAction();
        } else if (operation == "Follow") {
            if (isFollowingOn() == true)
                stopFollowingUnit();
            else
                startFollowingUnit(child.unitIndex);
        } else
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
    var child = units.item.children[index];
    var scheduledOperation = child.scheduledOperation;

    if ((scheduledOperation != "Ambush")
            && (scheduledOperation != "Defend")
            && (scheduledOperation != "Stop")
            && (scheduledOperation != "Follow")) {
        // Set up the unit to which the aimLine is anchored.
        // Others are set in the loop later, based on this "main"
        // object.
        issueActionOrder(child, targetX, targetY);

        for (var i = 0; i < children.length; i++) {
            child = children[i];

            // This unit's order is already issued.
            if (child.unitIndex == index)
                continue;

            // Sets schedule for all units.
            child.scheduledOperation = scheduledOperation;

            var tempX = targetX + (child.x - units.item.children[index].x);
            var tempY = targetY + (child.y - units.item.children[index].y);

            issueActionOrder(child, tempX, tempY);
        }
    }
    cleanContextAction();
}

function issueActionOrder(child, x, y) {
    var operation = child.scheduledOperation;

    // Clear defence, if it is on.
    child.defenceSphereColor = "";
    child.changeStatus("READY");

    if (operation == "Move") {
        child.moveTo(x, y);
    } else if (operation == "Move fast") {
        child.moveFastTo(x, y);
    } else if (operation == "Sneak") {
        child.sneakTo(x, y);
    } else if (operation == "Attack") {
        child.fireTo(x, y);
    } else if (operation == "Smoke") {
        child.smokeTo(x, y);
    }
    child.actionFinished.connect(actionFinished);

    setOrderMarker(child.unitIndex, operation, x, y);
}

function modifyTargetFromMarker(unitIndex) {
    var marker = orderMarkersContainer[unitIndex];
    var newX = marker.x + marker.centerX;
    var newY = marker.y + marker.centerY;
    var unit = units.item.children[unitIndex];

    unit.cancelOrder();
    issueActionOrder(unit, newX, newY);
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
        effectsContainer[i].source = "qrc:/img/effects/" + effectsContainer[i].animationString
                + (imgNumber + 1) + ".png";
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

function handleLeftMouseClick(mouse) {
        if (contextLoader.visible == false) {
            performContextAction(__unitIndex, mouseAreaMain.mouseX, mouseAreaMain.mouseY);
            return;
        } else {
            cleanContextAction();
            selectUnitFromGameArea(mouse);
        }
}

function handleRightMouseClick(mouse) {
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
            var mappedCoords = root.mapFromItem(gameArea,
                                                (child.x + child.centerX) * zoom,
                                                (child.y + child.centerY) * zoom);
            setContextMenuPosition(contextLoader,
                                   mappedCoords.x - (gameArea.contentX),
                                   mappedCoords.y - (gameArea.contentY));

            __unitIndex = childIndex(child);
            // Displays the context menu. This is suboptimal.
            contextLoader.source = "qrc:/skin/ContextMenu.qml";
            contextLoader.item.unitIndex = __unitIndex;
            contextLoader.item.menuEntryClicked.connect(scheduleContextAction);
        }
}

function handleLeftMouseClickRoster(mouse) {
        if (contextLoader.visible == false) {
//            performContextAction(mouseAreaMain.mouseX, mouseAreaMain.mouseY);
//            return;
        } else {
            cleanContextAction();
            selectUnitFromRoster(mouse);
        }
}

function handleRightMouseClickRoster(mouse) {
        cleanContextAction();

        var child;
        child = roster.childCenterCoords(mouse.x, mouse.y);
        var unit;
        unit = roster.getUnitAt(mouse.x, mouse.y);

        if (unit.centerX != undefined) {
            if (unit.selected == false) {
                selectUnitFromRoster(mouse);
            }
            // Fixes context menu at the centre of child object.
            setContextMenuPosition(contextLoader,
                                   menu.x + child.x,
                                   root.height + menu.y + child.y);

            __unitIndex = childIndex(unit);
            // Displays the context menu. This is suboptimal.
            contextLoader.source = "qrc:/skin/ContextMenu.qml";
            contextLoader.item.unitIndex = __unitIndex;
            contextLoader.item.menuEntryClicked.connect(scheduleContextAction);
        }
}

function handlePressAndHoldLeft(mouse) {
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

function handlePressAndHoldRight(mouse) {
    var infoString = map.item.terrainInfoString(mouse.x, mouse.y);
    terrainInfoText.text = infoString;
}

function handleMouseReleased() {
    if (rubberBandTimer.running == true) {
        rubberBandTimer.stop();
        rubberBand.visible = false;
        rubberBand.height = 2;
        rubberBand.width = 2;
    } else if (terrainInfoText.text != "") {
        terrainInfoText.text = "";
    }
}

function handleKeyPress(event) {
    if (event.modifiers == Qt.ControlModifier) {
        var digit = digitPressed(event);
        if (digit != -1)
            groupUnits(digit);
    } else {
        // Development key bindings.
        if (event.key == Qt.Key_BracketRight) {
            map.item.hipsometricMapInFront = !map.item.hipsometricMapInFront;
        } else
        // end of dev key bindings
        if (event.key == keyForFunction("zoom in")) {
            zoomIn();
        } else if (event.key == keyForFunction("zoom out")) {
            zoomOut();
        } else if (event.key == keyForFunction("follow")) {
            var selectedOnes = selectedUnits();
            if ((followedUnit.running == false) && (selectedUnitsCount() > 0)) {
                __unitIndex = selectedOnes[0].unitIndex;
                startFollowingUnit(__unitIndex);
            } else if (followedUnit.running == true) {
                if (selectedUnitsCount() > 0) {
                    __unitIndex = selectedOnes[0].unitIndex;

                    if (followedUnit.index == __unitIndex) {
                        stopFollowingUnit();
                    } else {
                        startFollowingUnit(__unitIndex);
                    }
                } else {
                    stopFollowingUnit();
                }
            } else {
                console.log("No unit selected to follow.");
            }
        } else if (ScenarioLogic.selectedUnitsCount() > 0) {
            var selectedOnes = selectedUnits();
            if (event.key == keyForFunction("Stop")) {
                for (var i = 0; i < selectedOnes.length; i++) {
                    selectedOnes[i].cancelOrder();
                    calculateOrderMarkerVisibility(selectedOnes[i].unitIndex);
                }
            } else if (event.key == keyForFunction("Move fast")) {
                __unitIndex = selectedOnes[0].unitIndex;
                scheduleContextAction(__unitIndex, "Move fast");
            } else if (event.key == keyForFunction("Move")) {
                __unitIndex = selectedOnes[0].unitIndex;
                scheduleContextAction(__unitIndex, "Move");
            } else if (event.key == keyForFunction("Sneak")) {
                __unitIndex = selectedOnes[0].unitIndex;
                scheduleContextAction(__unitIndex, "Sneak");
            } else if (event.key == keyForFunction("Attack")) {
                __unitIndex = selectedOnes[0].unitIndex;
                scheduleContextAction(__unitIndex, "Attack");
            } else if (event.key == keyForFunction("Smoke")) {
                __unitIndex = selectedOnes[0].unitIndex;
                scheduleContextAction(__unitIndex, "Smoke");
            } else if (event.key == keyForFunction("Defend")) {
                __unitIndex = selectedOnes[0].unitIndex;
                scheduleContextAction(__unitIndex, "Defend");
            } else if (event.key == keyForFunction("Ambush")) {
                __unitIndex = selectedOnes[0].unitIndex;
                scheduleContextAction(__unitIndex, "Ambush");
            }
        }

        if (event.key == keyForFunction("pause")) {
            togglePause();
        }

        // Digit reading.
        var digit = digitPressed(event);
        if (digit != -1)
            selectGroup(digit);
    }
}

function centerViewOnUnit(unit) {
    gameArea.contentX = (((unit.x + unit.centerX) * zoom) - gameArea.width/2);
    gameArea.contentY = (((unit.y + unit.centerY) * zoom) - gameArea.height/2);
}

function centerViewOn(x, y) {
    gameArea.contentX = (x - gameArea.width/2);
    gameArea.contentY = (y - gameArea.height/2);
}

function startFollowingUnit(index) {
    followedUnit.index = index;
    followedUnit.running = true;
    followingInfoBox.bodyText = "Unit name: " + units.item.children[index].unitType
            + "\nDouble click to stop.";

    if (followingTimer.running == false)
        followingTimer.start();
}

function stopFollowingUnit() {
    followedUnit.index = -1;
    followedUnit.running = false;

    if (followingTimer.running == true)
        followingTimer.stop();
}

function isFollowingOn() {
    return followedUnit.running;
}

function updateFollowingUnit() {
    var unit = units.item.children[followedUnit.index];
    centerViewOnUnit(unit);
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
    if ((modifier == Qt.NoModifier) && (uiMode == "DESKTOP")) {
        deselectAllUnits();
        units.item.children[index].selected = true;
        soldierMenu.populateSoldiers(units.item.children[index].soldiers);
    } else if ((modifier == Qt.ControlModifier) || (uiMode == "MOBILE")) {
        if (units.item.children[index].selected == true)
            units.item.children[index].selected = false;
        else if (units.item.children[index].selected == false)
            units.item.children[index].selected = true;

        if (selectedUnitsCount() > 1) {
            soldierMenu.clear();
        } else if (selectedUnitsCount() == 1) {
            soldierMenu.populateSoldiers(selectedUnits()[0].soldiers);
        }
    }

    calculateOrderMarkerVisibility(index);
}

function deselectUnit(index) {
    units.item.children[index].selected = false;
    calculateOrderMarkerVisibility(index);
//    soldierMenu.clear();
}

function deselectAllUnits() {
    soldierMenu.clear();
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
//    console.log("Group " + groupNumber + " created.");
    statusMessage("Group " + groupNumber + " created.");
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

    if ((child.unitStatus == "READY")
            || (child.unitStatus == "DEFENDING")
            || (child.unitStatus == "AMBUSHING")) {
        orderMarker.visible = false;
    } else {
        orderMarker.visible = true;
    }
}

function setOrderMarker(index, orderName, targetX, targetY) {
    var orderMarker = orderMarkersContainer[index];
    orderMarker.x = (targetX - orderMarker.centerX);
    orderMarker.y = (targetY - orderMarker.centerY);
    orderMarker.orderColor = LogicHelpers.colorForOrder(orderName);
    orderMarker.visible = true;
}

function createOrderMarkers() {
    for (var i = 0; i < units.item.children.length; i++) {
        // This component renders an order marker.
        var component = Qt.createComponent("qrc:/skin/OrderMarker.qml");
        var marker;

        if (component.status == Component.Ready) {
            marker = component.createObject(itemContainer);
            marker.visible = false;
            marker.index = i;
            marker.dragComplete.connect(modifyTargetFromMarker);
            orderMarkersContainer.push(marker);
        }
    }
}
