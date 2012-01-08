import QtQuick 1.1
import "../../qml/gui/menuEntries"

Rectangle {
    property color backgroundColor: "#bb333232"

    id: root
    color: backgroundColor
    border.color: Qt.tint(backgroundColor, "#88aaaaaa")
    border.width: 1

    width: configWindowWidth
    height: configWindowHeight
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
        screenSize.setWidth(configWindowWidth);
        screenSize.setHeight(configWindowHeight);
        maximised.checked = configMaximised;
        rememberDimensions.checked = configRememberDimensions;
    }

    function savePreferences() {
        uiMode = uimode.modeToShow;
        configMaximised = maximised.checked;
        configRememberDimensions = rememberDimensions.checked;
        if ((configWindowWidth != screenSize.getWidth())
                || (configWindowHeight != screenSize.getHeight())) {
            configWindowWidth = screenSize.getWidth();
            configWindowHeight = screenSize.getHeight();
            sizeModifiedInGame(screenSize.getWidth(), screenSize.getHeight());
        }

        // Keyboard shortcuts
        for (var i = 0; i < shortcuts.count; i++) {
            shortcuts.currentIndex = i;
            setConfigShortcut(shortcuts.currentItem.text, shortcuts.currentItem.getText());
        }

        toggleVisibility();
    }

    function populateShortcuts() {
        var shortcutsList = configShortcutNamesList();
        var valuesList = configShortcutValuesList();

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
            width: screenSize.width

            PreferencesCheckboxEntry {
                property string modeToShow: uiMode

                id: uimode
                text: "Ui mode: " + modeToShow;
                width: parent.width

                onCheckedChanged: {
                    var newUiMode = ((checked)? "DESKTOP" : "MOBILE");
                    modeToShow = newUiMode;
                }
            }

            PreferencesCheckboxEntry {
                id: maximised
                text: "Maximised"
                width: parent.width
            }

            PreferencesCheckboxEntry {
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
