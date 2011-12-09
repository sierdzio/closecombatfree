import QtQuick 1.1
import "../gui"
import "../engineLogicHelpers.js" as LogicHelpers
import "engineScenarioLogic.js" as ScenarioLogic

Item {
    property string scenarioFile: ""
    property int __aimLineRotation: 0
    property int __rubberBandRotation: 0
    property int __unitIndex: -1
    property real zoom: 1.0
    property bool paused: false
    property color menuBackgroundColor: "#7e8c24"

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
        ScenarioLogic.handleKeyPress(event);
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

                map.item.setUnits(units.item.children);
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

        Flickable {
            id: menu
            visible: false
            anchors.left: bottomMenu.left
            height: roster.height
            flickableDirection: Flickable.HorizontalFlick
            contentHeight: roster.height

            Row {
                anchors.fill: parent

                move: Transition {
                    NumberAnimation {
                        properties: "x"
                    }
                }

                ContextMenu {
                    // A static context menu, useful on mobile, where there is no right click.
                    id: contextMenu
                    height: menu.height
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

                    Behavior on opacity { NumberAnimation {} }
                }

                SoldierMenu {
                    id: soldierMenu
                    opacity: (empty)? 0 : 1;
                    backgroundColor: menuBackgroundColor
                    z: roster.z - 1

                    Behavior on opacity { NumberAnimation {} }
                }

                StatusMessageMenu {
                    id: statusMessageViewer
                    height: menu.height
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
        anchors.top: gameArea.top
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
