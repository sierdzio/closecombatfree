import QtQuick 1.1
import "../gui"
import "../maps"
import "../base/engineLogicHelpers.js" as LogicHelpers
import "engineScenarioLogic.js" as ScenarioLogic

Item {
    property string scenarioFile: ""
    property int __aimLineRotation: 0
    property int __rubberBandRotation: 0
    property int __unitIndex: -1
    property real zoom: 1.0
    property bool paused: false

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

    Component.onCompleted: {
        ScenarioLogic.createOrderMarkers();
        // If it' desktop, menus should be unrolled:
        if (uiMode == "DESKTOP") {
            topMenu.openMenu();
            bottomMenu.openMenu();
        }
    }

    Keys.onPressed: {
        if (event.modifiers == Qt.ControlModifier) {
            var digit = ScenarioLogic.digitPressed(event);
            if (digit != -1)
                ScenarioLogic.groupUnits(digit);
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
                if ((followedUnit.running == false) && (ScenarioLogic.selectedUnitsCount() > 0)) {
                    __unitIndex = ScenarioLogic.selectedUnits()[0].unitIndex;
                    ScenarioLogic.startFollowingUnit(__unitIndex);
                } else if (followedUnit.running == true) {
                    if (ScenarioLogic.selectedUnitsCount() > 0) {
                        __unitIndex = ScenarioLogic.selectedUnits()[0].unitIndex;

                        if (followedUnit.index == __unitIndex) {
                            ScenarioLogic.stopFollowingUnit();
                        } else {
                            ScenarioLogic.startFollowingUnit(__unitIndex);
                        }
                    } else {
                        ScenarioLogic.stopFollowingUnit();
                    }
                } else {
                    console.log("No unit selected to follow.");
                }
            } else if (ScenarioLogic.selectedUnitsCount() > 0) {
                // Would be good to optimise order key handling into a function
                if (event.key == keyForFunction("Stop")) {
                    var selectedUnits = ScenarioLogic.selectedUnits();
                    for (var i = 0; i < selectedUnits.length; i++) {
                        selectedUnits[i].cancelOrder();
                        ScenarioLogic.calculateOrderMarkerVisibility(selectedUnits[i].unitIndex);
                    }
                } else if (event.key == keyForFunction("Move fast")) {
                    __unitIndex = ScenarioLogic.selectedUnits()[0].unitIndex;
                    ScenarioLogic.scheduleContextAction(__unitIndex, "Move fast");
                } else if (event.key == keyForFunction("Move")) {
                    __unitIndex = ScenarioLogic.selectedUnits()[0].unitIndex;
                    ScenarioLogic.scheduleContextAction(__unitIndex, "Move");
                } else if (event.key == keyForFunction("Sneak")) {
                    __unitIndex = ScenarioLogic.selectedUnits()[0].unitIndex;
                    ScenarioLogic.scheduleContextAction(__unitIndex, "Sneak");
                } else if (event.key == keyForFunction("Attack")) {
                    __unitIndex = ScenarioLogic.selectedUnits()[0].unitIndex;
                    ScenarioLogic.scheduleContextAction(__unitIndex, "Attack");
                } else if (event.key == keyForFunction("Smoke")) {
                    __unitIndex = ScenarioLogic.selectedUnits()[0].unitIndex;
                    ScenarioLogic.scheduleContextAction(__unitIndex, "Smoke");
                } else if (event.key == keyForFunction("Defend")) {
                    __unitIndex = ScenarioLogic.selectedUnits()[0].unitIndex;
                    ScenarioLogic.scheduleContextAction(__unitIndex, "Defend");
                } else if (event.key == keyForFunction("Ambush")) {
                    __unitIndex = ScenarioLogic.selectedUnits()[0].unitIndex;
                    ScenarioLogic.scheduleContextAction(__unitIndex, "Ambush");
                }
            }

            if (event.key == keyForFunction("pause")) {
                togglePause();
            }

            // Digit reading.
            var digit = ScenarioLogic.digitPressed(event);
            if (digit != -1)
                ScenarioLogic.selectGroup(digit);
        }
    }

    Flickable {
        id: gameArea
        height: parent.height - bottomMenu.visibleHeight
        contentWidth: map.width
        contentHeight: map.height
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        scale: zoom

        anchors.top: root.top
        anchors.left: root.left
        anchors.right: root.right

        Loader {
            id: map
            source: units.item.mapFile
        }

        Loader {
            id: units
            anchors.fill: parent
            source: scenarioFile            

            Component.onCompleted: {
                for (var i = 0; i < units.item.children.length; i++) {
                    units.item.children[i].unitIndex = i;
                    togglePause.connect(units.item.children[i].togglePause);
                }
            }
        }

        MouseArea {
            id: mouseAreaMain
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
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
                if (uiMode == "DESKTOP") {
                    gameArea.interactive = false;
                    ScenarioLogic.handlePressAndHold(mouse);
                } else if (uiMode == "MOBILE") {
                    ScenarioLogic.handleRightMouseClick(mouse);
                }
            }
            onReleased: {
                if (uiMode == "DESKTOP") {
                    ScenarioLogic.handleMouseReleased();
                    gameArea.interactive = true;
                }
            }
            onDoubleClicked: {
                ScenarioLogic.stopFollowingUnit();
            }
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

        RubberBand {
            id: rubberBand
            visible: false

            transform: Rotation {
                origin.x: 0
                origin.y: 0
                angle: __rubberBandRotation
            }
        }

        // Needed for effectsContainer in JS file.
        // Would be neat to rethink and optimise that.
        Item {
            id: itemContainer
            visible: true
            anchors.fill: parent
        }
    }

    BottomMenu {        
        id: bottomMenu
        anchors.bottom: root.bottom
        anchors.right: root.right
        anchors.left: root.left
        visibleHeight: -menu.y

//        onVisibleHeightChanged: console.log("Visible height: " + visibleHeight
//                                            + "\nMenu y: " + menu.y
//                                            + "\nGame area height: " + gameArea.height);

        Flickable {
            id: menu
            visible: false
            anchors.left: bottomMenu.left
            height: roster.height
            width: bottomMenu.width
            flickableDirection: Flickable.HorizontalFlick
            contentWidth: roster.width + soldierMenu.width + statusMessageViewer.width
            contentHeight: roster.height
            clip: true

            Row {
                anchors.fill: parent

                RosterMenu {
                    id: roster

                    Component.onCompleted: {
                        populateUnits(units.item.children);
                    }

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
                }

                SoldierMenu {
                    id: soldierMenu
                }

                StatusMessageMenu {
                    id: statusMessageViewer
                    height: menu.height
                }
            }
        }
    }

    Loader {
        id: contextLoader
        rotation: 0
        z: roster.z + 1
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
        }
    }

    Item {
        property int index: -1
        property bool running: false

        id: followedUnit
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

    function childAt(x, y) {
        return units.item.childAt(x, y);
    }

    function childIndexAt(x, y) {
        var result = 0;
        var child = units.item.childAt(x, y);

        for (var i = 0; i < units.item.children.length; i++) {
            if (child == units.item.children[i]) {
                result = i;
                break;
            }
        }

        return result;
    }

    function childIndex(child) {
        var result = 0;

        for (var i = 0; i < units.item.children.length; i++) {
            if (child == units.item.children[i]) {
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

    // ------------------
    // Testing area.
//    Rectangle {
//        id: test1
//        color: "#ffff22"
//        width: 15
//        height: 15
//    }

//    Rectangle {
//        id: test2
//        color: "#ff22ff"
//        width: 15
//        height: 15
//    }
}
