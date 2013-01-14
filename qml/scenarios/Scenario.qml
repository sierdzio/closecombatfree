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
import QmlBase 0.1

/*!
  Base component for displaying scenarios. Inherits from CcfQmlBaseScenario.
  */
BaseScenario {
    id: root
    focus: true;

    anchors.left: parent.left
    anchors.top: parent.top

    // This is a temp name to avoid name clash.
    function saveGameToFile() {
        GameManager.saveGame(units, unitsLoader.item.mapFile);
    }

    Keys.onPressed: {
        handleKeyPress(event);
    }

    onZoomChanged: {
        zoomPoint = Qt.point(gameArea.width/2 + gameArea.contentX,
                             gameArea.height/2 + gameArea.contentY);

        gameArea.resizeContent((map.width * zoom), (map.height * zoom), zoomPoint);
        gameArea.returnToBounds();
    }

    /*!
      Main game area. Displays the map and units.
      */
    Flickable {
        objectName: "gameArea"
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
            objectName: "zoomArea"
            id: zoomArea
            scale: zoom

            Loader {
                property alias mapItem: map.item
                objectName: "map"
                id: map

                onLoaded: {
                    if (scenarioFile != "") {
                        height = map.item.height;
                        width = map.item.width;
                    }
                }

                Loader {
                    property alias unitsItem: unitsLoader.item
                    objectName: "unitsLoader"
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

                    onLoaded: init();
                }

                MouseArea {
                    objectName: "mouseAreaMain"
                    id: mouseAreaMain
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.Wheel
                    hoverEnabled: true
                    z: -1

                    onClicked: {
                        if (mouse.button == Qt.LeftButton) {
                            handleLeftMouseClick(mouse);
                        } else if (mouse.button == Qt.RightButton) {
                            handleRightMouseClick(mouse);
                        }
                    }

                    onPressAndHold: {
                        if (Config.uiMode == "DESKTOP") {
                            gameArea.interactive = false;
                            if (mouse.button == Qt.LeftButton) {
                                handlePressAndHoldLeft(mouse);
                            } else if (mouse.button == Qt.RightButton) {
                                handlePressAndHoldRight(mouse);;
                            }
                        } else if (Config.uiMode == "MOBILE") {
                            if (terrainInfoMode == "OFF") {
                                handleRightMouseClick(mouse);
                            } else {
                                handlePressAndHoldRight(mouse);
                            }
                        }
                    }

                    onReleased: {
                        if (Config.uiMode == "DESKTOP") {
                            handleMouseReleased();
                            gameArea.interactive = true;
                        }
                    }

                    onDoubleClicked: {
                        if (followingTimer.index != -1) {
                            stopFollowingUnit();
                        } else {
                            // Especially useful on mobile, where right click is not possible
                            handleRightMouseClick(mouse);
                        }
                    }

                    onWheel: {
                            handleWheelEventMouseAreaMain(wheel);
                    }
                }

                Rectangle {
                    property real obscureBeginning: height
                    property real invisibleBeginning: height
                    property string scheduledOperation: ""
                    property int unitIndex: -1

                    objectName: "aimLine"
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
                    property int rubberBandRotation: 0
                    objectName: "rubberBand"
                    id: rubberBand
                    visible: false
                    z: root.z + 1

                    transform: Rotation {
                        origin.x: 0
                        origin.y: 0
                        angle: rubberBand.rubberBandRotation
                    }
                }
            }
        }
    }

    BottomMenu {
        objectName: "bottomMenu"
        id: bottomMenu
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        visibleHeight: -menu.y

        Flickable {
            objectName: "menu"
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
                    id: contextMenuMobile
                    height: roster.height
                    backgroundColor: menuBackgroundColor
                    buttonHeight: ((height/9) - 1)
                    opacity: (Config.uiMode === "MOBILE")? 1: 0;
                    visible: (Config.uiMode === "MOBILE")? true: false;

                    Component.onCompleted: {
                        contextMenuMobile.menuEntryClicked.connect(scheduleContextAction);
                    }

                    Behavior on opacity { NumberAnimation {} }
                }

                RosterMenu {
                    objectName: "roster"
                    id: roster
                    backgroundColor: menuBackgroundColor
                    z: contextMenuMobile.z - 1

                    MouseArea {
                        id: mouseAreaRoster
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton

                        onClicked: {
                            if (mouse.button == Qt.LeftButton) {
                                handleLeftMouseClickRoster(mouse);
                            } else if (mouse.button == Qt.RightButton) {
                                handleRightMouseClickRoster(mouse);
                            }
                        }

                        onDoubleClicked: {
                            cleanContextAction();
                            var unit = roster.getUnitAt(mouse.x, mouse.y);
                            centerViewOnUnit(unit);
                        }
                    }

                    Behavior on opacity { NumberAnimation {} }
                }

                SoldierMenu {
                    objectName: "soldierMenu"
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

    ContextMenu {
        objectName: "contextMenu"
        id: contextMenu
        x: 0
        y: 0
        z: roster.z + 1
        visible: false
    }

    Text {
        objectName: "terrainInfoText"
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
            objectName: "followingInfoBox"
            id: followingInfoBox
            opacity: (followingTimer.running)? 1 : 0
            headerText: "Following:"
            bodyText: ""
            Behavior on opacity { NumberAnimation {} }
        }
    }

    TopMenu {
        objectName: "topMenu"
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
        objectName: "loadGame"
        id: loadGame
        anchors.fill: parent
        visible: false
    }

    AboutMenu {
        id: aboutMenu
        anchors.fill: parent
        visible: false
    }

    // Timer for visibility updates
    Timer {
        objectName: "sceneUpdateTimer"
        id: sceneUpdateTimer
        interval: 1000
        running: true
        repeat: true
    }

    // Timer for aimline rotation updates.
    Timer {
        objectName: "rotationTimer"
        id: rotationTimer
        interval: 120
        running: false
        repeat: true
    }

    // Timer for rubber band
    Timer {
        objectName: "rubberBandTimer"
        id: rubberBandTimer
        interval: 120
        running: false
        repeat: true
    }

    // Timer for on-screen animations..
    Timer {
        objectName: "effectsTimer"
        id: effectsTimer
        interval: 80
        running: false
        repeat: true
        triggeredOnStart: true
    }

    // Timer for unit following..
    Timer {
        property int index: -1

        objectName: "followingTimer"
        id: followingTimer
        interval: 20
        running: false
        repeat: true
        triggeredOnStart: true
    }
}
