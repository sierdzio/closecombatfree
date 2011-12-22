import QtQuick 1.1
import "qrc:/skin"
import "qrc:/core/engineLogicHelpers.js" as LogicHelpers
import "qrc:/core/engineScenarioLogic.js" as ScenarioLogic

Rectangle {
    property string scenarioFile: ""
    property int __aimLineRotation: 0
    property int __rubberBandRotation: 0
    property int __unitIndex: -1
    property real zoom: 1.0
    property variant zoomPoint: Qt.point(gameArea.width/2 + gameArea.contentX,
                                         gameArea.height/2 + gameArea.contentY); //Qt.point(0, 0)
    property bool paused: false
    property color menuBackgroundColor: "#7e8c24"

    signal closeScenario()
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
    color: menuBackgroundColor

    onScenarioFileChanged: {
        if (scenarioFile != "") {
        // If it' desktop, menus should be unrolled:
        if (uiMode == "DESKTOP") {
            topMenu.openMenu();
            bottomMenu.openMenu();
        }

        configWindowWidthChanged.connect(updateWidth);
        updateWidth();
        topMenu.save.connect(saveGameToFile);
        loadGame.gameEntryClicked.connect(loadScenario);
        }
    }

    // This is a temp name to avoid name clash.
    function saveGameToFile() {
        saveGame(units.item.children, units.item.mapFile);
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
            if ((bottomMenu.width < root.width) || (uiMode == "MOBILE"))
                return (parent.height - bottomMenu.visibleHeight);
            else
                return parent.height;
        }

        contentWidth: map.width
        contentHeight: map.height
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        anchors.top: root.top
        anchors.left: root.left
        anchors.right: root.right

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
                    id: units
                    anchors.fill: parent
                    z: map.z + 1
                    source: {
                        if (scenarioFile == "") {
                            return "";
                        } else if ((scenarioFile.charAt(0) != 'q') && (scenarioFile.charAt(0) != ':')) {
                            disableQrcUse(units);
                            return scenarioFile;
                        } else {
                            return scenarioFile;
                        }
                    }

                    onLoaded: {
                        if (scenarioFile != "") {
                            map.source = units.item.mapFile

                            for (var i = 0; i < units.item.children.length; i++) {
                                units.item.children[i].unitIndex = i;
                                togglePause.connect(units.item.children[i].togglePause);
                                units.item.children[i].actionFinished.connect(ScenarioLogic.actionFinished);
                            }

                            map.item.setUnits(units.item.children);
                            // Creates base for order markers.
                            ScenarioLogic.initOrderMarkers();
                            roster.populateUnits(units.item.children);
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
                            if (mouse.button == Qt.LeftButton) {
                                ScenarioLogic.handlePressAndHoldLeft(mouse);
                            } else if (mouse.button == Qt.RightButton) {
                                ScenarioLogic.handlePressAndHoldRight(mouse);;
                            }
                        } else if (uiMode == "MOBILE") {
                            if (terrainInfoMode == "OFF") {
                                ScenarioLogic.handleRightMouseClick(mouse);
                            } else {
                                ScenarioLogic.handlePressAndHoldRight(mouse);
                            }
                        }
                    }
                    onReleased: {
                        if (uiMode == "DESKTOP") {
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
                    z: root.z + 1

                    transform: Rotation {
                        origin.x: 0
                        origin.y: 0
                        angle: __rubberBandRotation
                    }
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
    }

    BottomMenu {
        id: bottomMenu
        anchors.bottom: root.bottom
        anchors.right: root.right
        anchors.left: root.left
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
                    opacity: (uiMode == "MOBILE")? 1: 0;

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
                    backgroundColor: menuBackgroundColor
                    z: roster.z - 1

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

//        onGameEntryClicked: {
//            parent.loadScenario(gamePath);
//        }
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

    function updateWidth() {
        if (configWindowWidth < menu.contentWidth) {
            menu.width = configWindowWidth;
        } else {
            menu.width = menu.contentWidth;
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
