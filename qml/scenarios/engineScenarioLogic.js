/****************************************************************************
** Close Combat Free is a free, easily moddable CC-like game.
** Copyright (C) 2011 Tomasz Siekierda
** Email: sierdzio@gmail.com, website: http://www.sierdzio.com
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program (in doc/ directory).
** If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
****************************************************************************/

/*!
  \ingroup CloseCombatFree

  \class engineScenarioLogic

  JavaScript file containing logic responsible for helping Scenario.qml do it's
  work. A lot of helper functions, as well as those crucial for game's operation.
  */

/*!
  \memberof engineScenarioLogic

  Schedules action chosen in context menu (or through a keyboard shortcut).
  It is then used to add that order to queue.
  */
function scheduleContextAction(index, operation) {
    var units;
    var unit;

    units = selectedUnits();
    if (unitsLoader.item.children[index] !== undefined) {
        unit = unitsLoader.item.children[index];
    } else {
        unit = units[0];
        __unitIndex = unit.unitIndex;
    }

    unit.scheduledOperation = operation;
    contextLoader.source = "";
    contextLoader.visible = false;

    // Prevents some strange errors in certain situations.
    if (unit.centerY !== undefined) {
        if (operation === "Stop") {
            // Iterate over every unit!
            for (var i = 0; i < units.length; i++) {
                units[i].cancelOrder();
                calculateOrderMarkerVisibility(units[i].unitIndex);
            }
            cleanContextAction();
        } else if (operation === "Follow") {
            if (isFollowingOn() === true)
                stopFollowingUnit();
            else
                startFollowingUnit(unit.unitIndex);
        } else
            // Draw aim line for all move/attack operations.
            if ((operation !== "Ambush") && (operation !== "Defend")) {
                aimLine.x = unit.x + unit.centerX;
                aimLine.y = unit.y + unit.centerY;

                aimLine.color = EngineHelpers.colorForOrder(operation);
                rotationTimer.start();
                aimLine.visible = true;

            } else { // Draw defense 'spheres'
                if (operation === "Ambush") {
                    for (var i = 0; i < units.length; i++) {
                        units[i].defenceSphereColor = "green";
                        units[i].changeStatus("AMBUSHING");
                    }
                }
                else if (operation === "Defend") {
                    for (var i = 0; i < units.length; i++) {
                        units[i].defenceSphereColor = "blue";
                        units[i].changeStatus("DEFENDING");
                    }
                }
                rotationTimer.start();
            }
    }
}

/*!
  \memberof engineScenarioLogic

  Cancels order if there are obstacles in LOS.
  */
function checkIfUnitCanFire(scheduledOperation) {
    // Take obstacles into account
    if ((scheduledOperation === "Attack") || (scheduledOperation === "Smoke")) {
        if (aimLine.invisibleBeginning < aimLine.height) {
            cleanContextAction();
            return false;
        }
    }

    return true;
}

/*!
  \memberof engineScenarioLogic

  Called when user "places order", that is, when they click left mouse button
  or tap on the screen.
  */
function performContextAction(index, targetX, targetY) {
    var selectedGroup = selectedUnits();
    var unit = unitsLoader.item.children[index];
    var scheduledOperation = unit.scheduledOperation;

    if ((scheduledOperation !== "Ambush")
            && (scheduledOperation !== "Defend")
            && (scheduledOperation !== "Stop")
            && (scheduledOperation !== "Follow")) {
        if (checkIfUnitCanFire(scheduledOperation) === false) {
            return;
        }

        // Set up the unit to which the aimLine is anchored.
        // Others are set in the loop later, based on this "main"
        // object.
        issueActionOrder(unit, targetX, targetY);

        for (var i = 0; i < selectedGroup.length; i++) {
            unit = selectedGroup[i];

            // This unit's order is already issued.
            if (unit.unitIndex === index)
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

/*!
  \memberof engineScenarioLogic

  Puts a waypoint on the map, also adds order to unit's order queue.
  */
function placeWaypoint(index, targetX, targetY) {
    var selectedGroup = selectedUnits();
    var unit = unitsLoader.item.children[index];
    var scheduledOperation = unit.scheduledOperation;

    if ((scheduledOperation !== "Ambush")
            && (scheduledOperation !== "Defend")
            && (scheduledOperation !== "Stop")
            && (scheduledOperation !== "Follow")) {
        if (checkIfUnitCanFire(scheduledOperation) === false) {
            return;
        }

        // Set up the unit to which the aimLine is anchored.
        // Others are set in the loop later, based on this "main"
        // object.
        issueWaypointOrder(unit, targetX, targetY);


        for (var i = 0; i < selectedGroup.length; i++) {
            unit = selectedGroup[i];

            // This unit's order is already issued.
            if (unit.unitIndex === index)
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

/*!
  \memberof engineScenarioLogic

  Issues a waypoint order (adds order to unit's queue).
  */
function issueWaypointOrder(unit, x, y) {
    var operation = unit.scheduledOperation;

    // WARNING! Order canceling IS important!
    //    unit.cancelOrder();

    // Clear defence, if it is on.
    unit.defenceSphereColor = "";
    unit.changeStatus("READY");

    unit.queueOrder(operation, x, y);

    setOrderMarker(unit.unitIndex, unit.orderQueue.length - 1, operation, x, y);
}

/*!
  \memberof engineScenarioLogic

  Issues an order (fires queue execution).
  */
function issueActionOrder(unit, x, y) {
    var operation = unit.scheduledOperation;

    // WARNING! Order canceling IS important!
    //    unit.cancelOrder();

    // Clear defence, if it is on.
    unit.defenceSphereColor = "";
    unit.changeStatus("READY");

    if (operation === "Move") {
        unit.moveTo(x, y);
    } else if (operation === "Move fast") {
        unit.moveFastTo(x, y);
    } else if (operation === "Sneak") {
        unit.sneakTo(x, y);
    } else if (operation === "Attack") {
        unit.fireTo(x, y);
    } else if (operation === "Smoke") {
        unit.smokeTo(x, y);
    }

    setOrderMarker(unit.unitIndex, unit.orderQueue.length - 1, operation, x, y);
}

/*!
  \memberof engineScenarioLogic

  Slot invoked when an unit finishes an action.
  */
function actionFinished(index, targetX, targetY) {
    var unit = unitsLoader.item.children[index];

    if (unit.currentOrder !== -1) {
        var scheduledOperation = unit.orderQueue[unit.currentOrder].operation;
        if ((scheduledOperation !== "Move")
                && (scheduledOperation !== "Move fast")
                && (scheduledOperation !== "Sneak")
                && (scheduledOperation !== "Follow")) {
            firingActionFinished(index, targetX, targetY);
            checkScenarioFinished();
        }

        calculateOrderMarkerVisibility(index);
    }
}

/*!
  \memberof engineScenarioLogic

  Checks whether scenario objectives are all met, and displays relevan info when
  it's needed.
  */
function checkScenarioFinished() {
    //// Experimental - unit destruction detection
    // It's probable that this should be done elsewhere.
    var areAllEnemiesDestroyed = true;
    var areAllAlliesDestroyed = true;
    for (var i = 0; i < unitsLoader.item.children.length; ++i) {
        var currentUnit = unitsLoader.item.children[i];
        if ((currentUnit.unitSide !== playerSide) && (currentUnit.state === "healthy")) {
            areAllEnemiesDestroyed = false;
        } else if ((currentUnit.unitSide === playerSide) && (currentUnit.state === "healthy")) {
            areAllAlliesDestroyed = false;
        }
    }

    if (scenarioWinStatus == "no") {
        if (areAllEnemiesDestroyed) {
            scenarioWinStatus = "won";
            Global.statusMessage("All enemies destroyed. You have won!");
        } else if (areAllAlliesDestroyed) {
            scenarioWinStatus = "lost";
            Global.statusMessage("All allies destroyed. You have lost!");
        }
    }
}

/*!
  \memberof engineScenarioLogic

  Invoked when a firing action is finished.

  Turns hit effects on, checks whether someone was hit, etc.
  */
function firingActionFinished(index, targetX, targetY) {
    var unit = unitsLoader.item.children[index];

    if (unit.currentOrder !== -1) {
        // This component renders in-game effects (not all,
        // but for example muzzle flashes, explosions etc.)
        var component = Qt.createComponent("../../qml/effects/Effect.qml");

        // A good place to include terrain recognition
        // for landing shells
        var effect;

        if (component.status === Component.Ready) {
            effect = component.createObject(itemContainer);
        }

        effectsContainer.push(effect);
        var scheduledOperation = unit.orderQueue[unit.currentOrder].operation;

        if (scheduledOperation === "Attack") {
            effect.animationString = "gun_fire";

            // Check, whether a unit was hit.
            var hitee = childAt(targetX, targetY);
            if ((hitee !== null) && (hitee.unitIndex !== undefined) && (hitee.unitType !== undefined)) {
                hitee.hit(unit.unitType, targetX, targetY);
            }
        }
        else if(scheduledOperation === "Smoke") {
            effect.animationString = "smoke_fire";
        }

        effect.x = targetX;
        effect.y = targetY;
        effect.running = true;

        if (effectsTimer.running == false)
            effectsTimer.start();
    }
}

/*!
  \memberof engineScenarioLogic

  Triggered by effectsTimer, switches all effects' frames to the next one.
  */
function updateEffects() {
    var haveAllEffectsFinished = true;
    for (var i = 0; i < effectsContainer.length; i++) {
        if (effectsContainer[i].running === true) {
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

/*!
  \memberof engineScenarioLogic

  Switches frame of a single effect to next one.
  */
function switchEffectFrame(effectIndex) {
    var i = effectIndex;
    var imgNumber = effectsContainer[i].imageNumber;

    if (imgNumber !== 5) {
        effectsContainer[i].imageNumber = imgNumber + 1;
        effectsContainer[i].source = "../../img/effects" + "/" + effectsContainer[i].animationString
                + (imgNumber + 1) + ".png";
    } else if (imgNumber === 5) {
        effectsContainer[i].imageNumber = 0;
        effectsContainer[i].source = "";
        effectsContainer[i].running = false;
    }
}

/*!
  \memberof engineScenarioLogic

  Cleans context action (hides context menu and aimLine, etc.).
  */
function cleanContextAction() {
    rotationTimer.stop();
    aimLine.visible = false;
    aimLine.height = 5;
    __unitIndex = -1;
    contextLoader.source = "";
    contextLoader.visible = true;
}

/*!
  \memberof engineScenarioLogic

  Returns Array of all units with exception of one, specified by unitIndex.

  If unitIndex is -1, this function returns ALL units.
  */
function getAllUnitsButOne(unitIndex) {
    var allUnits = unitsLoader.item.children;

    if (unitIndex === -1)
        return allUnits;

    var result = new Array(allUnits.length - 1);

    for (var i = 0; i < allUnits.length; ++i) {
        if (allUnits[i].unitIndex !== unitIndex) {
            result.push(allUnits[i]);
        }
    }

    return result;
}

/*!
  \memberof engineScenarioLogic

  Updates aimLine (rotation, length, anchor points, obscuring etc.).

  This function is called by aimLineTimer on every update.
  */
function updateAimLine() {
    var unit = unitsLoader.item.children[__unitIndex];

    if (aimLine.visible == true) {
        var x1 = unit.x + unit.centerX;
        var y1 = unit.y + unit.centerY;
        var x2 = mouseAreaMain.mouseX;
        var y2 = mouseAreaMain.mouseY;

        aimLine.x = x1;
        aimLine.y = y1;

        var newRotation = EngineHelpers.rotationAngle(x2, y2, x1, y1);

        if (__aimLineRotation !== newRotation) {
            __aimLineRotation = newRotation;
            aimLine.height = EngineHelpers.targetDistance(x1, y1, x2, y2);

            // If obscuring should be turned off for some actions (movement)
            // an if clause here would do the trick.
            var terrainObscure = Terrain.checkForTerrainInLOS(x1, y1, x2, y2, unit);
            var propsObscure = EngineHelpers.checkForObstaclesInLOS(map.item.getProps(),
                                                                   x1, y1, x2, y2, unit);

            var unitsObscure = 0;
            var targetUnit = childAt(x2, y2);
            if ((targetUnit === undefined) || (targetUnit === null)) { // Operation should be checked here!
                targetUnit = -1;
            } else {
                unitsObscure = EngineHelpers.checkForObstaclesInLOS(getAllUnitsButOne(targetUnit.unitIndex),
                                                                   x1, y1, x2, y2, unit);
            }

            // Conditions here should be redesigned to save time.
            // There is no need to update aimLine if a given Beginning
            // is further than current one.
            if (terrainObscure !== 0) {
                if (terrainObscure < 0) {
                    aimLine.obscureBeginning = -terrainObscure;
                } else {
                    aimLine.invisibleBeginning = terrainObscure;
                }
            } else {
                aimLine.obscureBeginning = aimLine.height;
                aimLine.invisibleBeginning = aimLine.height;
            }

            if (propsObscure !== 0) {
                if (propsObscure < 0) {
                    aimLine.obscureBeginning = -propsObscure;
                } else {
                    aimLine.invisibleBeginning = propsObscure;
                }
            }

            if (unitsObscure !== 0) {
                if (unitsObscure < 0) {
                    aimLine.obscureBeginning = -unitsObscure;
                } else {
                    aimLine.invisibleBeginning = unitsObscure;
                }
            }
        }
    } else {
        var tempRotation;
        tempRotation = EngineHelpers.rotationAngle(unit.x + unit.centerX,
                                                  unit.y + unit.centerY,
                                                  mouseAreaMain.mouseX,
                                                  mouseAreaMain.mouseY);
        unit.defenceSphereRotation = unit.rotation
                + EngineHelpers.angleTo8Step(tempRotation);
    }
}

/*!
  \memberof engineScenarioLogic

  Cancels all orders for all selected units.
  */
function cancelAllSelectedOrders() {
    var selected = selectedUnits();
    for (var i = 0; i < selectedUnitsCount(); ++i) {
        selected[i].cancelOrder();
    }
}

/*!
  \memberof engineScenarioLogic

  Handles left mouse click.
  */
function handleLeftMouseClick(mouse) {
    if (contextLoader.visible == false) {
        if (mouse.modifiers === Qt.ShiftModifier) {
            placeWaypoint(__unitIndex, mouseAreaMain.mouseX, mouseAreaMain.mouseY);
        } else {
            performContextAction(__unitIndex, mouseAreaMain.mouseX, mouseAreaMain.mouseY);
//            return;
        }
    } else {
        cleanContextAction();
        selectUnitFromGameArea(mouse);
    }
}

/*!
  \memberof engineScenarioLogic

  Handles right mouse click.
  */
function handleRightMouseClick(mouse) {
    cleanContextAction();
    if (aimLine.visible == true)
        cancelAllSelectedOrders();

    var unit;
    unit = childAt(mouseAreaMain.mouseX, mouseAreaMain.mouseY);

    if ((unit === mouseAreaMain) || (unit === null) || (unit.unitSide !== playerSide)) {
        deselectAllUnits();
        return;
    }

    if (unit.centerX !== undefined) {
        if (unit.selected === false) {
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
        contextLoader.source = "../../qml/gui/ContextMenu.qml";
        contextLoader.item.unitIndex = __unitIndex;
        contextLoader.item.menuEntryClicked.connect(scheduleContextAction);
    }
}

/*!
  \memberof engineScenarioLogic

  Handles left mouse click on roster menu.
  */
function handleLeftMouseClickRoster(mouse) {
    if (contextLoader.visible == false) {
        //            performContextAction(mouseAreaMain.mouseX, mouseAreaMain.mouseY);
        //            return;
    } else {
        cleanContextAction();
        selectUnitFromRoster(mouse);
    }
}

/*!
  \memberof engineScenarioLogic

  Handles right mouse click on roster menu.
  */
function handleRightMouseClickRoster(mouse) {
    cleanContextAction();

    var unit;
    unit = roster.getUnitAt(mouse.x, mouse.y);

    if (unit.centerX !== undefined) {
        if (unit.selected === false) {
            selectUnitFromRoster(mouse);
        }

        var rosterUnitCoords;
        rosterUnitCoords = roster.childCenterCoords(mouse.x, mouse.y);
        // Fixes context menu at the centre of unit object.
        setContextMenuPosition(contextLoader,
                               menu.x + rosterUnitCoords.x,
                               root.height + menu.y + rosterUnitCoords.y);

        __unitIndex = childIndex(unit);
        // Displays the context menu. This is suboptimal.
        contextLoader.source = "../../qml/gui/ContextMenu.qml";
        contextLoader.item.unitIndex = __unitIndex;
        contextLoader.item.menuEntryClicked.connect(scheduleContextAction);
    }
}

/*!
  \memberof engineScenarioLogic

  Handles "press and hold" action of left mouse button.
  */
function handlePressAndHoldLeft(mouse) {
    rubberBand.x = mouse.x;
    rubberBand.y = mouse.y;
    rubberBand.height = 2;
    rubberBand.width = 2;
    rubberBand.visible = true;

    // Saves the state of modifiers.
    //    rubberBandTimer.__modifiers = mouse.modifiers;

    if (mouse.modifiers === Qt.NoModifier)
        deselectAllUnits();

    if (rubberBandTimer.running == false)
        rubberBandTimer.start();
}

/*!
  \memberof engineScenarioLogic

  Handles "press and hold" action of right mouse button.
  */
function handlePressAndHoldRight(mouse) {
    var infoString = map.item.terrainInfoString(mouse.x, mouse.y);
    terrainInfoText.text = infoString;
}

/*!
  \memberof engineScenarioLogic

  Invoked when mouse button is released.
  */
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

/*!
  \memberof engineScenarioLogic

  Handles key press.

  Uses C++ bindings heavily to check keyboard shortcuts in CcfConfig.
  */
function handleKeyPress(event) {
    var digit = digitPressed(event);
    if (event.modifiers === Qt.ControlModifier) {
        if (digit !== -1)
            groupUnits(digit);
    } else {
        var selectedOnes;
        // Development key bindings.
        if (event.key === Qt.Key_BracketRight) {
            map.item.hipsometricMapInFront = !map.item.hipsometricMapInFront;
        } else if (event.key === Qt.Key_BracketLeft) {
            togglePlayer();
        } else
            // end of dev key bindings
            if (event.key === Config.keyForFunction("zoom in")) {
                zoomIn();
            } else if (event.key === Config.keyForFunction("zoom out")) {
                zoomOut();
            } else if (event.key === Config.keyForFunction("quit")) {
                Qt.quit();
            } else if (event.key === Config.keyForFunction("toggle top menu")) {
                topMenu.toggleMenu();
            } else if (event.key === Config.keyForFunction("toggle bottom menu")) {
                bottomMenu.toggleMenu();
            } else if (event.key === Config.keyForFunction("follow")) {
                selectedOnes = selectedUnits();
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
            } else if (selectedUnitsCount() > 0) {
                selectedOnes = selectedUnits();
                if (event.key === Config.keyForFunction("Stop")) {
                    for (var i = 0; i < selectedOnes.length; i++) {
                        selectedOnes[i].cancelOrder();
                        calculateOrderMarkerVisibility(selectedOnes[i].unitIndex);
                    }
                } else if (event.key === Config.keyForFunction("Move fast")) {
                    __unitIndex = selectedOnes[0].unitIndex;
                    scheduleContextAction(__unitIndex, "Move fast");
                } else if (event.key === Config.keyForFunction("Move")) {
                    __unitIndex = selectedOnes[0].unitIndex;
                    scheduleContextAction(__unitIndex, "Move");
                } else if (event.key === Config.keyForFunction("Sneak")) {
                    __unitIndex = selectedOnes[0].unitIndex;
                    scheduleContextAction(__unitIndex, "Sneak");
                } else if (event.key === Config.keyForFunction("Attack")) {
                    __unitIndex = selectedOnes[0].unitIndex;
                    scheduleContextAction(__unitIndex, "Attack");
                } else if (event.key === Config.keyForFunction("Smoke")) {
                    __unitIndex = selectedOnes[0].unitIndex;
                    scheduleContextAction(__unitIndex, "Smoke");
                } else if (event.key === Config.keyForFunction("Defend")) {
                    __unitIndex = selectedOnes[0].unitIndex;
                    scheduleContextAction(__unitIndex, "Defend");
                } else if (event.key === Config.keyForFunction("Ambush")) {
                    __unitIndex = selectedOnes[0].unitIndex;
                    scheduleContextAction(__unitIndex, "Ambush");
                }
            }

        if (event.key === Config.keyForFunction("pause")) {
            togglePause();
        }

        // Digit reading.
        if (digit !== -1)
            selectGroup(digit);
    }
}

/*!
  \memberof engineScenarioLogic

  Handles key press.

  Uses C++ bindings heavily to check keyboard shortcuts in CcfConfig.
  */
function handleWheelEventMouseAreaMain(wheel) {
    if (wheel.modifiers === Qt.ControlModifier) {
        zoom += wheel.angleDelta.y/800;
    } else if (wheel.modifiers === Qt.ShiftModifier) {
        gameArea.flick(wheel.angleDelta.y * 6, 0.001);
    } else {
        wheel.accepted = false;
    }
}

/*!
  \memberof engineScenarioLogic

  Centers view on a given unit.
  */
function centerViewOnUnit(unit) {
    gameArea.contentX = (((unit.x + unit.centerX) * zoom) - gameArea.width/2);
    gameArea.contentY = (((unit.y + unit.centerY) * zoom) - gameArea.height/2);
}

/*!
  \memberof engineScenarioLogic

  Centers view on given coordinates.
  */
function centerViewOn(x, y) {
    gameArea.contentX = (x - gameArea.width/2);
    gameArea.contentY = (y - gameArea.height/2);
}

/*!
  \memberof engineScenarioLogic

  Starts following a unit specified by it's index.
  */
function startFollowingUnit(index) {
    followedUnit.index = index;
    followedUnit.running = true;
    centerViewOnUnit(index);
    followingInfoBox.bodyText = "Unit name: " + unitsLoader.item.children[index].unitType
            + "\nDouble click to stop.";

    if (followingTimer.running == false)
        followingTimer.start();
}

/*!
  \memberof engineScenarioLogic

  Stops following a unit.
  */
function stopFollowingUnit() {
    followedUnit.index = -1;
    followedUnit.running = false;

    if (followingTimer.running == true)
        followingTimer.stop();
}

/*!
  \memberof engineScenarioLogic

  Returns true is any unit is being followed.
  */
function isFollowingOn() {
    return followedUnit.running;
}

/*!
  \memberof engineScenarioLogic

  Invoked on every followTimer update.

  Checks whether a unit is moving, centers view on the unit etc. If a unit is
  stationary, timer is turned off.
  */
function updateFollowingUnit() {
    var unit = unitsLoader.item.children[followedUnit.index];
    if (unit.moving === true) {
        centerViewOnUnit(unit);
    } else {
        followingTimer.stop();
    }
}

/*!
  \memberof engineScenarioLogic

  Restarts followTimer when a unit is on the move.
  */
function handleUnitMovement(isMoving, unitIndex) {
    if (followedUnit.index !== unitIndex) {
        return;
    }

    if (isMoving === true) {
        followingTimer.start();
    } else if (isMoving === false) {
        followingTimer.stop();
    }
}

/*!
  \memberof engineScenarioLogic

  Updates the rubber band (dimensions).
  */
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

        if (unit.selected === true)
            continue;

        if ((unit.x <= rubberX2) && (unit.x >= rubberX)
                && (unit.y <= rubberY2) && (unit.y >= rubberY)) {
            selectUnit(unit.unitIndex, Qt.ControlModifier);
        }
    }
}

/*!
  \memberof engineScenarioLogic

  Selects a unit from gameArea (as opposed to roster).
  */
function selectUnitFromGameArea(mouse) {
    var unit = childAt(mouse.x, mouse.y);

    if (unit === null) {
        deselectAllUnits();
        return;
    }

    if (unit.unitStatus !== undefined) {
        selectUnit(unit.unitIndex, mouse.modifiers);
    } else {
        deselectAllUnits();
    }
}

/*!
  \memberof engineScenarioLogic

  Selects a unit from roster menu.
  */
function selectUnitFromRoster(mouse) {
    var unit = roster.getUnitAt(mouse.x, mouse.y);

    if (unit !== 0) {
        selectUnit(unit.unitIndex, mouse.modifiers);
    }
}

/*!
  \memberof engineScenarioLogic

  Selects a single unit, based on some rules (it cannot be destroyed or belong
  to the enemy).
  */
function selectUnit(index, modifier) {
    if ((unitsLoader.item.children[index].unitSide !== playerSide)
            || (unitsLoader.item.children[index].state !== "healthy")) {
        return;
    }

    if ((modifier === Qt.NoModifier) && (Config.uiMode == "DESKTOP")) {
        deselectAllUnits();
        unitsLoader.item.children[index].selected = true;
        soldierMenu.populateSoldiers(unitsLoader.item.children[index].soldiers);
    } else if ((modifier === Qt.ControlModifier) || (Config.uiMode == "MOBILE")) {
        if (unitsLoader.item.children[index].selected === true)
            unitsLoader.item.children[index].selected = false;
        else if (unitsLoader.item.children[index].selected === false)
            unitsLoader.item.children[index].selected = true;

        if (selectedUnitsCount() > 1) {
            soldierMenu.clear();
        } else if (selectedUnitsCount() === 1) {
            soldierMenu.populateSoldiers(selectedUnits()[0].soldiers);
        }
    }

    calculateOrderMarkerVisibility(index);
}

/*!
  \memberof engineScenarioLogic

  Deselects a given unit.
  */
function deselectUnit(index) {
    unitsLoader.item.children[index].selected = false;
    calculateOrderMarkerVisibility(index);
    //    soldierMenu.clear();
}

/*!
  \memberof engineScenarioLogic

  Deselects all units.
  */
function deselectAllUnits() {
    soldierMenu.clear();
    var units = unitsLoader.item.children;
    for (var i = 0; i < units.length; i++) {
        deselectUnit(i);
    }
}

/*!
  \memberof engineScenarioLogic

  Returns number of units that are selected.
  */
function selectedUnitsCount() {
    var result = 0;
    var units = unitsLoader.item.children;
    for (var i = 0; i < units.length; i++) {
        if (units[i].selected === true)
            result ++;
    }
    return result;
}

/*!
  \memberof engineScenarioLogic

  Returns an array of all selected units.
  */
function selectedUnits() {
    var result = new Array();
    var units = unitsLoader.item.children;
    for (var i = 0; i < units.length; i++) {
        if (units[i].selected === true)
            result.push(units[i]);
    }
    return result;
}

/*!
  \memberof engineScenarioLogic

  Creates an unit group. Usually done with CTRL+number.
  */
function groupUnits(groupNumber) {
    if (selectedUnitsCount() === 0) {
        return;
    }

    // Remove old members.
    if (unitGroups[groupNumber] !== undefined) {
        for (var i = 0; i < unitGroups[groupNumber].length; i++) {
            if (unitGroups[groupNumber][i].selected === false)
                unitGroups[groupNumber][i].groupNumber = 0;
        }
    }

    var group = selectedUnits();
    unitGroups[groupNumber] = group;

    for (var i = 0; i < group.length; i++) {
        group[i].groupNumber = groupNumber;
    }
    //    console.log("Group " + groupNumber + " created.");
    Global.statusMessage("Group " + groupNumber + " created.");
}

/*!
  \memberof engineScenarioLogic

  Selects a whole unit group.

  It has to be created first, of course.
  */
function selectGroup(groupNumber) {
    var group = unitGroups[groupNumber];

    if (group !== undefined) {
        deselectAllUnits();
    } else {
        return;
    }

    for (var i = 0; i < group.length; i++) {
        group[i].selected = true;
    }
}

/*!
  \memberof engineScenarioLogic

  Correctly positions the context menu (so that it does not go beyond the screen).
  */
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

/*!
  \memberof engineScenarioLogic

  Handles digit presses, returns a number corresponding to digit that was pressed.
  */
function digitPressed(event) {
    var result = -1;

    if (event.key === Qt.Key_1)
        result = 1;
    else if (event.key === Qt.Key_2)
        result = 2;
    else if (event.key === Qt.Key_3)
        result = 3;
    else if (event.key === Qt.Key_4)
        result = 4;
    else if (event.key === Qt.Key_5)
        result = 5;
    else if (event.key === Qt.Key_6)
        result = 6;
    else if (event.key === Qt.Key_7)
        result = 7;
    else if (event.key === Qt.Key_8)
        result = 8;
    else if (event.key === Qt.Key_9)
        result = 9;
    else if (event.key === Qt.Key_0)
        result = 10;

    return result;
}

/*!
  \memberof engineScenarioLogic

  Checks which order markers should be visible and clears those no longer needed.
  */
function calculateOrderMarkerVisibility(index) {
    var orderMarker = orderMarkersContainer[index];
    var unit = unitsLoader.item.children[index];

    var anyOrdersLeft = false;
    var orders = unit.orderQueue;

    if (orders.length === 0) {
        for (var i = 0; i < orderMarker.length; i++) {
            if (orderMarker[i] !== 0) {
                orderMarker[i].destroy();
                orderMarker[i] = 0;
            }
        }
    } else {
        for (var i = 0; i < orders.length; i++) {
            if (orders[i].performed === true) {
                if (i < orderMarker.length) {
                    if (orderMarker[i] !== 0) {
                        orderMarker[i].destroy();
                        orderMarker[i] = 0;
                    }
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

/*!
  \memberof engineScenarioLogic

  Crreates an order marker and places it on game map.
  */
function setOrderMarker(index, orderNumber, orderName, targetX, targetY) {
    // This component renders an order marker.
    var component = Qt.createComponent("../../qml/gui/OrderMarker.qml");
    var marker;

    if (component.status === Component.Ready) {
        marker = component.createObject(itemContainer);
        marker.visible = true;
        marker.index = index;
        marker.number = orderNumber;
        marker.dragComplete.connect(modifyTargetFromMarker);
        orderMarkersContainer[index][orderNumber] = marker;
    }

    marker.x = (targetX - marker.centerX);
    marker.y = (targetY - marker.centerY);
    marker.orderColor = EngineHelpers.colorForOrder(orderName);
    marker.visible = true;
}

/*!
  \memberof engineScenarioLogic

  When user drags order marker about the game area, unit's orders are being updated.
  */
function modifyTargetFromMarker(unitIndex, orderNumber) {
    var marker = orderMarkersContainer[unitIndex][orderNumber];
    var newX = marker.x + marker.centerX;
    var newY = marker.y + marker.centerY;
    var unit = unitsLoader.item.children[unitIndex];

    // Not sure whether this should stay!
    //    unit.cancelOrder();
    unit.modifyOrder(orderNumber, newX, newY);
}

/*!
  \memberof engineScenarioLogic

  Initialises order markers' container.
  */
function initOrderMarkers() {
    for (var i = 0; i < unitsLoader.item.children.length; i++) {
        orderMarkersContainer[i] = new Array();
    }
}
