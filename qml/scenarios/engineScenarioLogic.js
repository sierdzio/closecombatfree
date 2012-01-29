var effectsContainer = new Array();
var orderMarkersContainer = new Array();
var unitGroups = new Array(10);

function scheduleContextAction(index, operation) {
    var units;
    var unit;

    units = selectedUnits();
    if (unitsLoader.item.children[index] != undefined) {
        unit = unitsLoader.item.children[index];
    } else {
        unit = units[0];
        __unitIndex = unit.unitIndex;
    }

    unit.scheduledOperation = operation;
    contextLoader.source = "";
    contextLoader.visible = false;

    // Prevents some strange errors in certain situations.
    if (unit.centerY != undefined) {
        if (operation == "Stop") {
            // Iterate over every unit!
            for (var i = 0; i < units.length; i++) {
                units[i].cancelOrder();
                calculateOrderMarkerVisibility(units[i].unitIndex);
            }
            cleanContextAction();
        } else if (operation == "Follow") {
            if (isFollowingOn() == true)
                stopFollowingUnit();
            else
                startFollowingUnit(unit.unitIndex);
        } else
            // Draw aim line for all move/attack operations.
            if ((operation != "Ambush") && (operation != "Defend")) {
                aimLine.x = unit.x + unit.centerX;
                aimLine.y = unit.y + unit.centerY;

                aimLine.color = LogicHelpers.colorForOrder(operation);
                rotationTimer.start();
                aimLine.visible = true;

            } else { // Draw defense 'spheres'
                if (operation == "Ambush") {
                    for (var i = 0; i < units.length; i++) {
                        units[i].defenceSphereColor = "green";
                        units[i].changeStatus("AMBUSHING");
                    }
                }
                else if (operation == "Defend") {
                    for (var i = 0; i < units.length; i++) {
                        units[i].defenceSphereColor = "blue";
                        units[i].changeStatus("DEFENDING");
                    }
                }
                rotationTimer.start();
            }
    }
}

function performContextAction(index, targetX, targetY) {
    var selectedGroup = selectedUnits();
    var unit = unitsLoader.item.children[index];
    var scheduledOperation = unit.scheduledOperation;

    if ((scheduledOperation != "Ambush")
            && (scheduledOperation != "Defend")
            && (scheduledOperation != "Stop")
            && (scheduledOperation != "Follow")) {
        // Set up the unit to which the aimLine is anchored.
        // Others are set in the loop later, based on this "main"
        // object.
        issueActionOrder(unit, targetX, targetY);

        for (var i = 0; i < selectedGroup.length; i++) {
            unit = selectedGroup[i];

            // This unit's order is already issued.
            if (unit.unitIndex == index)
                continue;

            // Sets schedule for all units.
            unit.scheduledOperation = scheduledOperation;

            var tempX = targetX + (unit.x - unitsLoader.item.children[index].x);
            var tempY = targetY + (unit.y - unitsLoader.item.children[index].y);

            issueActionOrder(unit, tempX, tempY);
        }
    }
    cleanContextAction();
}

function placeWaypoint(index, targetX, targetY) {
    var selectedGroup = selectedUnits();
    var unit = unitsLoader.item.children[index];
    var scheduledOperation = unit.scheduledOperation;

    if ((scheduledOperation != "Ambush")
            && (scheduledOperation != "Defend")
            && (scheduledOperation != "Stop")
            && (scheduledOperation != "Follow")) {
        // Set up the unit to which the aimLine is anchored.
        // Others are set in the loop later, based on this "main"
        // object.
        issueWaypointOrder(unit, targetX, targetY);


        for (var i = 0; i < selectedGroup.length; i++) {
            unit = selectedGroup[i];

            // This unit's order is already issued.
            if (unit.unitIndex == index)
                continue;

            // Sets schedule for all units.
            unit.scheduledOperation = scheduledOperation;

            var tempX = targetX + (unit.x - unitsLoader.item.children[index].x);
            var tempY = targetY + (unit.y - unitsLoader.item.children[index].y);

            issueWaypointOrder(unit, tempX, tempY);
        }
    }
    //    cleanContextAction();
}

function issueWaypointOrder(unit, x, y) {
    var operation = unit.scheduledOperation;

    // WARNING! Order canceling IS important!
    //    unit.cancelOrder();

    // Clear defence, if it is on.
    unit.defenceSphereColor = "";
    unit.changeStatus("READY");

    unit.queueOrder(operation, x, y);

    setOrderMarker(unit.unitIndex, unit.getOrderQueue().length - 1, operation, x, y);
}

function issueActionOrder(unit, x, y) {
    var operation = unit.scheduledOperation;

    // WARNING! Order canceling IS important!
    //    unit.cancelOrder();

    // Clear defence, if it is on.
    unit.defenceSphereColor = "";
    unit.changeStatus("READY");

    if (operation == "Move") {
        unit.moveTo(x, y);
    } else if (operation == "Move fast") {
        unit.moveFastTo(x, y);
    } else if (operation == "Sneak") {
        unit.sneakTo(x, y);
    } else if (operation == "Attack") {
        unit.fireTo(x, y);
    } else if (operation == "Smoke") {
        unit.smokeTo(x, y);
    }

    setOrderMarker(unit.unitIndex, unit.getOrderQueue().length - 1, operation, x, y);
}

function actionFinished(index, targetX, targetY) {
    var unit = unitsLoader.item.children[index];

    if (unit.currentOrder != -1) {
        var scheduledOperation = unit.getOrderQueue()[unit.currentOrder].operation;
        if ((scheduledOperation != "Move")
                && (scheduledOperation != "Move fast")
                && (scheduledOperation != "Sneak")
                && (scheduledOperation != "Follow")) {
            firingActionFinished(index, targetX, targetY);
            checkScenarioFinished();
        }

        calculateOrderMarkerVisibility(index);
    }
}

function checkScenarioFinished() {
    //// Experimental - unit destruction detection
    // It's probable that this should be done elsewhere.
    var areAllEnemiesDestroyed = true;
    var areAllAlliesDestroyed = true;
    for (var i = 0; i < unitsLoader.item.children.length; ++i) {
        var currentUnit = unitsLoader.item.children[i];
        if ((currentUnit.unitSide != playerSide) && (currentUnit.state == "healthy")) {
            areAllEnemiesDestroyed = false;
        } else if ((currentUnit.unitSide == playerSide) && (currentUnit.state == "healthy")) {
            areAllAlliesDestroyed = false;
        }
    }

    if (scenarioWinStatus == "no") {
        if (areAllEnemiesDestroyed) {
            scenarioWinStatus = "won";
            statusMessage("All enemies destroyed. You have won!");
        } else if (areAllAlliesDestroyed) {
            scenarioWinStatus = "lost";
            statusMessage("All allies destroyed. You have lost!");
        }
    }
}

function firingActionFinished(index, targetX, targetY) {
    var unit = unitsLoader.item.children[index];

    if (unit.currentOrder != -1) {
        // This component renders in-game effects (not all,
        // but for example muzzle flashes, explosions etc.)
        var component = Qt.createComponent("../qml/scenarios/Effect.qml");

        // A good place to include terrain recognition
        // for landing shells
        var effect;

        if (component.status == Component.Ready) {
            effect = component.createObject(itemContainer);
        }

        effectsContainer.push(effect);
        var scheduledOperation = unit.getOrderQueue()[unit.currentOrder].operation;

        if (scheduledOperation == "Attack") {
            effect.animationString = "gun_fire";

            // Check, whether a unit was hit.
            var hitee = childAt(targetX, targetY);
            if ((hitee != null) && (hitee.unitIndex != undefined)) {
                hitee.hit(unit.unitType, targetX, targetY);
            }
        }
        else if(scheduledOperation == "Smoke") {
            effect.animationString = "smoke_fire";
        }

        effect.x = targetX;
        effect.y = targetY;
        effect.running = true;

        if (effectsTimer.running == false)
            effectsTimer.start();
    }
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
        effectsContainer[i].source = "../img/effects" + "/" + effectsContainer[i].animationString
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
    var unit = unitsLoader.item.children[__unitIndex];

    if (aimLine.visible == true) {
        var x1 = unit.x + unit.centerX;
        var y1 = unit.y + unit.centerY;
        var x2 = mouseAreaMain.mouseX;
        var y2 = mouseAreaMain.mouseY;

        aimLine.x = x1;
        aimLine.y = y1;

        var newRotation = LogicHelpers.rotationAngle(x2, y2, x1, y1);

        if (__aimLineRotation != newRotation) {
            __aimLineRotation = newRotation;
            aimLine.height = LogicHelpers.targetDistance(x1, y1, x2, y2);

            // If obscuring should be turned off for some actions (movement)
            // an if clause here would do the trick.
            var terrainObscure = checkForTerrainInLOS(x1, y1, x2, y2, unit);
            var propsObscure = LogicHelpers.checkForObstaclesInLOS(map.item.getProps(), x1, y1, x2, y2, unit);
            var unitsObscure = LogicHelpers.checkForObstaclesInLOS(unitsLoader.item.children, x1, y1, x2, y2, unit);

            // Conditions here should be redesigned to save time.
            // There is no need to update aimLine if a given Beginning
            // is further than current one.
            if (terrainObscure != 0) {
                if (terrainObscure < 0) {
                    aimLine.obscureBeginning = -terrainObscure;
                } else {
                    aimLine.invisibleBeginning = terrainObscure;
                }
            } else {
                aimLine.obscureBeginning = aimLine.height;
                aimLine.invisibleBeginning = aimLine.height;
            }

            if (propsObscure != 0) {
                if (propsObscure < 0) {
                    aimLine.obscureBeginning = -propsObscure;
                } else {
                    aimLine.invisibleBeginning = propsObscure;
                }
            }

            if (unitsObscure != 0) {
                if (unitsObscure < 0) {
                    aimLine.obscureBeginning = -unitsObscure;
                } else {
                    aimLine.invisibleBeginning = unitsObscure;
                }
            }
        }
    } else {
        var tempRotation;
        tempRotation = LogicHelpers.rotationAngle(unit.x + unit.centerX,
                                                  unit.y + unit.centerY,
                                                  mouseAreaMain.mouseX,
                                                  mouseAreaMain.mouseY);
        unit.defenceSphereRotation = unit.rotation
                + LogicHelpers.angleTo8Step(tempRotation);
    }
}

function cancelAllSelectedOrders() {
    var selected = selectedUnits();
    for (var i = 0; i < selectedUnitsCount(); ++i) {
        selected[i].cancelOrder();
    }
}

function handleLeftMouseClick(mouse) {
    if (contextLoader.visible == false) {
        if (mouse.modifiers == Qt.ShiftModifier) {
            placeWaypoint(__unitIndex, mouseAreaMain.mouseX, mouseAreaMain.mouseY);
        } else {
            performContextAction(__unitIndex, mouseAreaMain.mouseX, mouseAreaMain.mouseY);
            return;
        }
    } else {
        cleanContextAction();
        selectUnitFromGameArea(mouse);
    }
}

function handleRightMouseClick(mouse) {
    cleanContextAction();
    if (aimLine.visible == true)
        cancelAllSelectedOrders();

    var unit;
    unit = childAt(mouseAreaMain.mouseX, mouseAreaMain.mouseY);

    if ((unit == mouseAreaMain) || (unit == null) || (unit.unitSide != playerSide)) {
        deselectAllUnits();
        return;
    }

    if (unit.centerX != undefined) {
        if (unit.selected == false) {
            selectUnitFromGameArea(mouse);
        }

        // Fixes context menu at the centre of unit object.
        var mappedCoords = root.mapFromItem(gameArea,
                                            (unit.x + unit.centerX) * zoom,
                                            (unit.y + unit.centerY) * zoom);
        setContextMenuPosition(contextLoader,
                               mappedCoords.x - (gameArea.contentX),
                               mappedCoords.y - (gameArea.contentY));

        __unitIndex = childIndex(unit);
        // Displays the context menu. This is suboptimal.
        contextLoader.source = "../qml/gui/ContextMenu.qml";
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

    var unit;
    unit = roster.childCenterCoords(mouse.x, mouse.y);
    var unit;
    unit = roster.getUnitAt(mouse.x, mouse.y);

    if (unit.centerX != undefined) {
        if (unit.selected == false) {
            selectUnitFromRoster(mouse);
        }
        // Fixes context menu at the centre of unit object.
        setContextMenuPosition(contextLoader,
                               menu.x + unit.x,
                               root.height + menu.y + unit.y);

        __unitIndex = childIndex(unit);
        // Displays the context menu. This is suboptimal.
        contextLoader.source = "../qml/gui/ContextMenu.qml";
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
        } else if (event.key == Qt.Key_BracketLeft) {
            togglePlayer();
        } else
            // end of dev key bindings
            if (event.key == keyForFunction("zoom in")) {
                zoomIn();
            } else if (event.key == keyForFunction("zoom out")) {
                zoomOut();
            } else if (event.key == keyForFunction("toggle top menu")) {
                topMenu.toggleMenu();
            } else if (event.key == keyForFunction("toggle bottom menu")) {
                bottomMenu.toggleMenu();
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
    centerViewOnUnit(unit);
    followingInfoBox.bodyText = "Unit name: " + unitsLoader.item.children[index].unitType
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
    var unit = unitsLoader.item.children[followedUnit.index];
    if (unit.moving == true) {
        centerViewOnUnit(unit);
    } else {
        followingTimer.stop();
    }
}

function handleUnitMovement(isMoving, unitIndex) {
    if (followedUnit.index != unitIndex) {
        return;
    }

    if (isMoving == true) {
        followingTimer.start();
    } else if (isMoving == false) {
        followingTimer.stop();
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
    var units = unitsLoader.item.children;
    for (var i = 0; i < units.length; i++) {
        var unit = units[i];

        if (unit.selected == true)
            continue;

        if ((unit.x <= rubberX2) && (unit.x >= rubberX)
                && (unit.y <= rubberY2) && (unit.y >= rubberY)) {
            selectUnit(unit.unitIndex, Qt.ControlModifier);
        }
    }
}

function selectUnitFromGameArea(mouse) {
    var unit = childAt(mouse.x, mouse.y);

    if (unit == null) {
        deselectAllUnits();
        return;
    }

    if (unit.unitStatus != undefined) {
        selectUnit(unit.unitIndex, mouse.modifiers);
    } else {
        deselectAllUnits();
    }
}

function selectUnitFromRoster(mouse) {
    var unit = roster.getUnitAt(mouse.x, mouse.y);

    if (unit != 0) {
        selectUnit(unit.unitIndex, mouse.modifiers);
    }
}

function selectUnit(index, modifier) {
    if ((unitsLoader.item.children[index].unitSide != playerSide)
            || (unitsLoader.item.children[index].state != "healthy")) {
        return;
    }

    if ((modifier == Qt.NoModifier) && (uiMode == "DESKTOP")) {
        deselectAllUnits();
        unitsLoader.item.children[index].selected = true;
        soldierMenu.populateSoldiers(unitsLoader.item.children[index].soldiers);
    } else if ((modifier == Qt.ControlModifier) || (uiMode == "MOBILE")) {
        if (unitsLoader.item.children[index].selected == true)
            unitsLoader.item.children[index].selected = false;
        else if (unitsLoader.item.children[index].selected == false)
            unitsLoader.item.children[index].selected = true;

        if (selectedUnitsCount() > 1) {
            soldierMenu.clear();
        } else if (selectedUnitsCount() == 1) {
            soldierMenu.populateSoldiers(selectedUnits()[0].soldiers);
        }
    }

    calculateOrderMarkerVisibility(index);
}

function deselectUnit(index) {
    unitsLoader.item.children[index].selected = false;
    calculateOrderMarkerVisibility(index);
    //    soldierMenu.clear();
}

function deselectAllUnits() {
    soldierMenu.clear();
    var units = unitsLoader.item.children;
    for (var i = 0; i < units.length; i++) {
        deselectUnit(i);
    }
}

function selectedUnitsCount() {
    var result = 0;
    var units = unitsLoader.item.children;
    for (var i = 0; i < units.length; i++) {
        if (units[i].selected == true)
            result ++;
    }
    return result;
}

function selectedUnits() {
    var result = new Array();
    var units = unitsLoader.item.children;
    for (var i = 0; i < units.length; i++) {
        if (units[i].selected == true)
            result.push(units[i]);
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
    var unit = unitsLoader.item.children[index];

    var anyOrdersLeft = false;
    var orders = unit.getOrderQueue();

    if (orders.length == 0) {
        for (var i = 0; i < orderMarker.length; i++) {
            orderMarker[i].destroy();
        }
    } else {
        for (var i = 0; i < orders.length; i++) {
            if (orders[i].performed == true) {
                if (i < orderMarker.length) {
                    orderMarker[i].destroy();
                }
            } else {
                anyOrdersLeft = true;
            }
        }
    }

    // Clean markers on queue finish
    if (anyOrdersLeft == false) {
        orderMarker = new Array();
    }
}

function setOrderMarker(index, orderNumber, orderName, targetX, targetY) {
    // This component renders an order marker.
    var component = Qt.createComponent("../qml/gui/OrderMarker.qml");
    var marker;

    if (component.status == Component.Ready) {
        marker = component.createObject(itemContainer);
        marker.visible = true;
        marker.index = index;
        marker.number = orderNumber;
        marker.dragComplete.connect(modifyTargetFromMarker);
        orderMarkersContainer[index][orderNumber] = marker;
    }

    marker.x = (targetX - marker.centerX);
    marker.y = (targetY - marker.centerY);
    marker.orderColor = LogicHelpers.colorForOrder(orderName);
    marker.visible = true;
}

function modifyTargetFromMarker(unitIndex, orderNumber) {
    var marker = orderMarkersContainer[unitIndex][orderNumber];
    var newX = marker.x + marker.centerX;
    var newY = marker.y + marker.centerY;
    var unit = unitsLoader.item.children[unitIndex];

    // Not sure whether this should stay!
    //    unit.cancelOrder();
    unit.modifyOrder(orderNumber, newX, newY);
}

function initOrderMarkers() {
    for (var i = 0; i < unitsLoader.item.children.length; i++) {
        orderMarkersContainer[i] = new Array();
    }
}
