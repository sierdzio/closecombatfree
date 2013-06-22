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

import QtQuick 2.1
import QtQuick.Controls 1.0
import "../../../qml/gui/menuEntries"

Rectangle {
    property color backgroundColor: "#bb333232"

    id: root
    color: backgroundColor
    border.color: Qt.tint(backgroundColor, "#88aaaaaa")
    border.width: 1

    width: Config.windowWidth
    height: Config.windowHeight
    radius: 20

    Component.onCompleted: {
        closeButton.entryClicked.connect(toggleVisibility);
        saveButton.entryClicked.connect(savePreferences);

        fillPreferencesData();
        populateShortcuts();
    }

    function toggleVisibility() {
        root.visible = !root.visible;
    }

    function fillPreferencesData() {
        screenSize.setWidth(Config.windowWidth);
        screenSize.setHeight(Config.windowHeight);
        maximised.checked = Config.maximised;
        rememberDimensions.checked = Config.rememberDimensions;
    }

    function savePreferences() {
        Config.uiMode = uimode.modeToShow;
        Config.maximised = maximised.checked;
        Config.rememberDimensions = rememberDimensions.checked;
        if ((Config.windowWidth != screenSize.getWidth())
                || (Config.windowHeight != screenSize.getHeight())) {
            Config.windowWidth = screenSize.getWidth();
            Config.windowHeight = screenSize.getHeight();
            sizeModifiedInGame(screenSize.getWidth(), screenSize.getHeight());
        }

        // Keyboard shortcuts
        for (var i = 0; i < shortcuts.count; i++) {
            shortcuts.currentIndex = i;
            Config.setShortcut(shortcuts.currentItem.text, shortcuts.currentItem.getText());
        }

        toggleVisibility();
    }

    function populateShortcuts() {
        var shortcutsList = Config.shortcutNamesList();
        var valuesList = Config.shortcutValuesList();

        for (var i = 0; i < shortcutsList.length; i++) {
            var currentShortcut = shortcutsList[i];
            shortcutModel.append({"label": currentShortcut});
            shortcuts.currentIndex = i;
            shortcuts.currentItem.setText(valuesList[i]);
            shortcuts.currentItem.width = shortcuts.width;
        }
    }

    MouseArea {
        id: blockingArea
        anchors.fill: parent
    }

    Flickable {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: buttons.top
        anchors.topMargin: parent.radius
        anchors.leftMargin: parent.radius
        contentWidth: entries.width + shortcuts.width + 5
        contentHeight: shortcuts.contentHeight + 2
        clip: true
        interactive: ((contentWidth > width) || (contentHeight > height))? true: false;

        Column {
            id: entries

            spacing: 5
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 2
            anchors.bottom: parent.bottom
            width: screenSize.width + 40

//            PreferencesCheckboxEntry {
//                property string modeToShow: Config.uiMode

//                id: uimode
//                text: "Ui mode: " + modeToShow;
//                width: parent.width

//                onCheckedChanged: {
//                    var newUiMode = ((checked)? "DESKTOP" : "MOBILE");
//                    modeToShow = newUiMode;
//                }
//            }

//            PreferencesCheckboxEntry {
//                id: maximised
//                text: "Maximised"
//                width: parent.width
//            }

//            PreferencesCheckboxEntry {
//                id: rememberDimensions
//                text: "Remember window dimensions on exit"
//                width: parent.width
//                enabled: !maximised.checked

//                onEnabledChanged: {
//                    if(!enabled)
//                        checked = true;
//                }
//            }

            CheckBox {
                property string modeToShow: Config.uiMode

                id: uimode
                text: "Ui mode: " + modeToShow
                width: parent.width

                onCheckedChanged: {
                    modeToShow = ((checked)? "DESKTOP" : "MOBILE");
                }
            }

            CheckBox {
                id: maximised
                text: "Maximised"
                width: parent.width
            }

            CheckBox {
                id: rememberDimensions
                text: "Remember window dimensions on exit"
                width: parent.width
                enabled: !maximised.checked

                onEnabledChanged: {
                    if(!enabled)
                        checked = true;
                }
            }

            PreferencesScreenSizeEntry {
                id: screenSize
            }
        }

        // Shortcuts area
        ListModel {
            id: shortcutModel
        }

        Component {
            id: shortcutDelegate

            PreferencesSingleTextInputEntry { text: label }
        }

        GridView {
            id: shortcuts
            interactive: false

            anchors.top: parent.top
            anchors.left: entries.right
            anchors.topMargin: 2
            anchors.leftMargin: 5
            anchors.bottom: parent.bottom

            width: entries.width
            cellHeight: screenSize.height + 5
            cellWidth: entries.width

            model: shortcutModel
            delegate: shortcutDelegate
        }
    }

    // Bottom buttons
    Row {
        id: buttons

        spacing: 5
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: parent.radius
        anchors.rightMargin: parent.radius

        MenuEntry {
            id: saveButton
            text: "Save"
            additionalText: "Saves preferences to config file"
        }

        MenuEntry {
            id: closeButton
            text: "Close"
            additionalText: "This will cancel any changes\nmade since last save"
        }
    }
}
