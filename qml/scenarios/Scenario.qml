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

import QtQuick 2.0
import "../../qml/gui"
import "../../qml/scenarios/engineScenarioLogic.js" as ScenarioLogic
import QmlBase 0.1

BaseScenario {
//    // Holds graphical effects (muzzle flashes, hit animations etc.).
    property var effectsContainer: new Array;
//    // Holds unit groups (ones created with CTRL + digit).
    property var unitGroups: new Array(10);

    signal closeScenario() // TODO: clean containers to preserve memory
    signal loadScenario(string path)

    signal togglePause ()
    onTogglePause: {
        if (paused == true) {
            paused = false;
        } else {
            paused = true;
        }
    }

    id: root
    focus: true;

    anchors.left: parent.left
    anchors.top: parent.top

    onScenarioFileChanged: {
        if (scenarioFile != "") {
            // If it' desktop, menus should be unrolled:
            if (Config.uiMode == "DESKTOP") {
                topMenu.openMenu();
                // TODO: turn back on for release. It's off now because it's annoying ;)
//                bottomMenu.openMenu();
            }

            Config.windowWidthChanged.connect(updateWidth);
            updateWidth();
            topMenu.save.connect(saveGameToFile);
            loadGame.gameEntryClicked.connect(loadScenario);
        }
    }

    // This is a temp name to avoid name clash.
    function saveGameToFile() {
        GameManager.saveGame(units, unitsLoader.item.mapFile);
    }

    function playerUnits(player) {
        if (player === "") {
            return units;
        }

        var unitsArray = new Array;
        for (var i = 0; i < units.length; ++i) {
            if (units[i].unitSide === player) {
                unitsArray.push(units[i]);
            }
        }
        return unitsArray;
    }

    // TODO: make this a property! This method is invoked far too often!
    function enemyUnits(player) {
        if (player === "") {
            return units;
        }

        var unitsArray = new Array;
        for (var i = 0; i < units.length; ++i) {
            if (units[i].unitSide !== player) {
                unitsArray.push(units[i]);
            }
        }
        return unitsArray;
    }

    function togglePlayer() {
        var sides = new Array;
        // Find all available sides. TEMP!
        for (var i = 0; i < units.length; ++i) {
            if (EngineHelpers.arrayContains(sides, units[i].unitSide) === -1) {
                sides.push(units[i].unitSide);
            }
        }

        // Switch to next one in line.
        for (var j = 0; j < sides.length; ++j) {
            if (sides[j] === ScenarioState.playerSide) {
                if (j != (sides.length - 1)) {
                    ScenarioState.playerSide = sides[j + 1];
                } else {
                    ScenarioState.playerSide = sides[0];
                }

                Global.statusMessage("Player side changed to: " + ScenarioState.playerSide);
                break;
            }
        }

        hideNonPlayerUnits();
        scenarioWinStatus = "no";
        ScenarioLogic.checkScenarioFinished();
    }

    function hideNonPlayerUnits() {
        for (var i = 0; i < units.length; ++i) {
            if (units[i].unitSide !== ScenarioState.playerSide)
                units[i].visible = false;
        }
    }

    function setSideMarks() {
        for (var i = 0; i < units.length; ++i) {
            units[i].sideMarkSource = ScenarioState.getSidePath(units[i].unitSide);
        }
    }

    function updateWidth() {
        if (Config.windowWidth < menu.contentWidth) {
            menu.width = Config.windowWidth;
        } else {
            menu.width = menu.contentWidth;
        }
    }

    function childAt(x, y) {
        return unitsLoader.item.childAt(x, y);
    }

    function childIndexAt(x, y) {
        return childIndex(childAt(x, y));
    }

    function childIndex(child) {
        var result = 0;

        for (var i = 0; i < units.length; i++) {
            if (child === units[i]) {
                result = i;
                break;
            }
        }

        return result;
    }

    function zoomIn() {
        zoom += 0.1;
    }

    function zoomOut() {
        zoom -= 0.1;
    }

    Keys.onPressed: {
        ScenarioLogic.handleKeyPress(event);
    }

    onZoomChanged: {
        zoomPoint = Qt.point(gameArea.width/2 + gameArea.contentX,
                             gameArea.height/2 + gameArea.contentY);

        gameArea.resizeContent((map.width * zoom), (map.height * zoom), zoomPoint);
        gameArea.returnToBounds();
    }

    Flickable {
        id: gameArea
        height: {
            if ((bottomMenu.width < root.width) || (Config.uiMode == "MOBILE"))
                return (parent.height - bottomMenu.visibleHeight);
            else
                return parent.height;
        }

        contentWidth: map.width
        contentHeight: map.height
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        PinchArea {
            id: pinchArea
            anchors.fill: parent

            onPinchUpdated: {
                zoom += pinch.scale - pinch.previousScale;
            }
        }

        Item {
            id: zoomArea
            scale: zoom

            Loader {
                id: map

                onLoaded: {
                    if (scenarioFile != "") {
                        height = map.item.height;
                        width = map.item.width;
                    }
                }

                Loader {
                    id: unitsLoader
                    anchors.fill: parent
                    z: map.z + 1
                    source: {
                        if (scenarioFile == "") {
                            return "";
                            // IF the first char is 'q', we assume that QRC is in operation.
                        } else if ((scenarioFile.charAt(0) != 'q') && (scenarioFile.charAt(0) != ':')) {
                            Global.disableQrcUse(unitsLoader);
                            return scenarioFile;
                        } else {
                            return scenarioFile;
                        }
                    }

                    onLoaded: {
                        if (scenarioFile != "") {
                            var unitSideList = new Array;

                            if (unitsLoader.item.objectName != "Campaign") {
                                // This is a single scenario
                                map.source = unitsLoader.item.mapFile;
                                units = unitsLoader.item.children;

                                for (var i = 0; i < units.length; i++) {
                                    var unit = units[i];
                                    unit.unitIndex = i;
                                    togglePause.connect(unit.togglePause);
                                    unit.actionFinished.connect(ScenarioLogic.actionFinished);
                                    unit.movementStateChange.connect(ScenarioLogic.handleUnitMovement);
                                    unitSideList.push(unit.unitSide);
                                }

                                map.item.setUnits(units);
                            } else {
                                // This is a campaign
                                // TODO: add some clever code here ;)
                            }

                            roster.populateUnits(playerUnits(ScenarioState.playerSide));
                            hideNonPlayerUnits();
                            ScenarioState.setAvailableSides(unitSideList);
                            setSideMarks();
                        }
                    }
                }

                MouseArea {
                    id: mouseAreaMain
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.Wheel
                    hoverEnabled: true
                    z: -1

                    onClicked: {
                        if (mouse.button == Qt.LeftButton) {
                            ScenarioLogic.handleLeftMouseClick(mouse);
                        } else if (mouse.button == Qt.RightButton) {
                            ScenarioLogic.handleRightMouseClick(mouse);
                        }
                    }

                    onPressAndHold: {
                        if (Config.uiMode == "DESKTOP") {QQuickItem
                            gameArea.interactive = false;
                            if (mouse.button == Qt.LeftButton) {
                                ScenarioLogic.handlePressAndHoldLeft(mouse);
                            } else if (mouse.button == Qt.RightButton) {
                                ScenarioLogic.handlePressAndHoldRight(mouse);;
                            }
                        } else if (Config.uiMode == "MOBILE") {
                            if (terrainInfoMode == "OFF") {
                                ScenarioLogic.handleRightMouseClick(mouse);
                            } else {
                                ScenarioLogic.handlePressAndHoldRight(mouse);
                            }
                        }
                    }

                    onReleased: {
                        if (Config.uiMode == "DESKTOP") {
                            ScenarioLogic.handleMouseReleased();
                            gameArea.interactive = true;
                        }
                    }

                    onDoubleClicked: {
                        if (followedUnit.index != -1) {
                            ScenarioLogic.stopFollowingUnit();
                        } else {
                            // Especially useful on mobile, where right click is not possible
                            ScenarioLogic.handleRightMouseClick(mouse);
                        }
                    }

                    onWheel: {
                            ScenarioLogic.handleWheelEventMouseAreaMain(wheel);
                    }
                }

                Rectangle {
                    property real obscureBeginning: height
                    property real invisibleBeginning: height

                    id: aimLine
                    z: root.z + 1
                    visible: false
                    width: 3
                    height: 150

                    Rectangle {
                        id: aimLineObscured
                        z: aimLine.z + 1
                        width: parent.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        visible: parent.visible
                        color: Qt.darker(parent.color, 1.5)

                        height: parent.height - parent.obscureBeginning
                    }

                    Rectangle {
                        id: aimLineInvisible
                        z: aimLineObscured.z + 1
                        width: parent.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        visible: parent.visible
                        color: Qt.darker(parent.color, 2.0)

                        height: parent.height - parent.invisibleBeginning
                    }

                    transform: Rotation {
                        origin.x: 1
                        origin.y: 0
                        angle: aimLineRotation
                    }
                }

                RubberBand {
                    id: rubberBand
                    visible: false
                    z: root.z + 1

                    transform: Rotation {
                        origin.x: 0
                        origin.y: 0
                        angle: rubberBandRotation
                    }
                }
            }
        }
    }

    BottomMenu {
        id: bottomMenu
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        visibleHeight: -menu.y

        Flickable {
            id: menu
            visible: false
            anchors.left: bottomMenu.left
            pressDelay: 800
            flickableDirection: Flickable.HorizontalFlick

            height: row.height
            width: row.width
            contentHeight: row.height
            contentWidth: row.width

            Row {
                id: row

                move: Transition {
                    NumberAnimation {
                        properties: "x"
                    }
                }

                ContextMenu {
                    // A static context menu, useful on mobile, where there is no right click.
                    id: contextMenu
                    height: roster.height
                    backgroundColor: menuBackgroundColor
                    buttonHeight: ((height/9) - 1)
                    opacity: (Config.uiMode === "MOBILE")? 1: 0;
                    visible: (Config.uiMode === "MOBILE")? true: false;

                    Component.onCompleted: {
                        contextMenu.menuEntryClicked.connect(ScenarioLogic.scheduleContextAction);
                    }

                    Behavior on opacity { NumberAnimation {} }
                }

                RosterMenu {
                    id: roster
                    backgroundColor: menuBackgroundColor
                    z: contextMenu.z - 1

                    MouseArea {
                        id: mouseAreaRoster
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton

                        onClicked: {
                            if (mouse.button == Qt.LeftButton) {
                                ScenarioLogic.handleLeftMouseClickRoster(mouse);
                            } else if (mouse.button == Qt.RightButton) {
                                ScenarioLogic.handleRightMouseClickRoster(mouse);
                            }
                        }

                        onDoubleClicked: {
                            ScenarioLogic.cleanContextAction();
                            var unit = roster.getUnitAt(mouse.x, mouse.y);
                            ScenarioLogic.centerViewOnUnit(unit);
                        }
                    }

                    Behavior on opacity { NumberAnimation {} }
                }

                SoldierMenu {
                    id: soldierMenu
                    height: roster.height
                    opacity: (empty)? 0 : 1;
                    visible: (empty)? false : true;
                    backgroundColor: menuBackgroundColor
                    z: roster.z - 1

                    MouseArea {
                        anchors.fill: parent
                    }

                    Behavior on opacity { NumberAnimation {} }
                }

                StatusMessageMenu {
                    id: statusMessageViewer
                    height: roster.height
                    opacity: (empty)? 0 : 1;
                    backgroundColor: menuBackgroundColor
                    z: soldierMenu.z - 1

                    Behavior on opacity { NumberAnimation {} }
                }
            }
        }
    }

    Loader {
        id: contextLoader
        rotation: 0
        z: roster.z + 1
    }

    Text {
        id: terrainInfoText
        text: ""
        visible: (text == "")? false : true;

        height: paintedHeight
        width: paintedWidth
        anchors.left: gameArea.left
        anchors.top: topMenu.bottom
        color: "#ddeeee00"
        font.pointSize: 14
        style: Text.Raised
        font.bold: true
        font.family: "Ubuntu Condensed"
        verticalAlignment: Text.Center
        horizontalAlignment: Text.AlignLeft
    }

    Row {
        id: infoboxHolder
        anchors.bottom: gameArea.bottom
        anchors.horizontalCenter: gameArea.horizontalCenter

        SimpleInfoBox {
            y: (infoboxHolder.height - height) / 2
            opacity: paused? 1 : 0
            headerText: "Pause activated"
            bodyText: "This is active pause. You can still give orders."
            Behavior on opacity { NumberAnimation {} }
        }

        SimpleInfoBox {
            id: followingInfoBox
            opacity: (followedUnit.running)? 1 : 0
            headerText: "Following:"
            bodyText: ""
            Behavior on opacity { NumberAnimation {} }
        }
    }

    TopMenu {
        id: topMenu
        anchors.top: gameArea.top
        anchors.right: gameArea.right
        currentZoom: (zoom * 100)

        Component.onCompleted: {
            topMenu.pauseEntryClicked.connect(togglePause);
            topMenu.zoomIn.connect(root.zoomIn);
            topMenu.zoomOut.connect(root.zoomOut);
            topMenu.closeScenario.connect(root.closeScenario);
            topMenu.preferences.connect(togglePreferences);
            topMenu.load.connect(loadGame.toggleVisibility);
            topMenu.about.connect(aboutMenu.toggleVisibility);
        }
    }

    function togglePreferences() {
        preferences.visible = !preferences.visible;
    }

    PreferencesMenu {
        id: preferences
        anchors.fill: parent
        visible: false
    }

    LoadGameMenu {
        id: loadGame
        anchors.fill: parent
        visible: false
    }

    AboutMenu {
        id: aboutMenu
        anchors.fill: parent
        visible: false
    }

    Item {
        property int index: -1
        property bool running: false

        id: followedUnit
    }

    // Timer for visibility updates
    Timer {
        id: sceneUpdateTimer
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            ScenarioLogic.updateUnitVisibility();
        }
    }

    // Timer for aimline rotation updates.
    Timer {
        id: rotationTimer
        interval: 120
        running: false
        repeat: true
        onTriggered: {
            ScenarioLogic.updateAimLine();
        }
    }

    // Timer for rubber band
    Timer {
        id: rubberBandTimer
        interval: 120
        running: false
        repeat: true
        onTriggered: {
            ScenarioLogic.updateRubberBand(mouseAreaMain.mouseX, mouseAreaMain.mouseY);
        }
    }

    // Timer for on-screen animations..
    Timer {
        id: effectsTimer
        interval: 80
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            ScenarioLogic.updateEffects();
        }
    }

    // Timer for unit following..
    Timer {
        id: followingTimer
        interval: 20
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            ScenarioLogic.updateFollowingUnit();
        }
    }

    // ------------------
    // Testing area.
//    Rectangle {
//        id: test1
//        color: "#ffff22"
//        width: 15
//        height: 15

//        Behavior on x { NumberAnimation {} }
//        Behavior on y { NumberAnimation {} }
//    }

//    Rectangle {
//        id: test2
//        color: "#ff22ff"
//        width: 15
//        height: 15
//    }
}
