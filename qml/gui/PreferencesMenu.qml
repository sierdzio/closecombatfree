import QtQuick 1.1
import "qrc:/skin/menuEntries"

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
        configWindowWidth = screenSize.getWidth();
        configWindowHeight = screenSize.getHeight();

        toggleVisibility();
    }

    MouseArea {
        id: blockingArea
        anchors.fill: parent
    }

    Column {
        id: entries

        spacing: 5
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: parent.radius
        anchors.leftMargin: parent.radius
        anchors.bottom: buttons.top
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
            enabled: !rememberDimensions.checked
        }
    }

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
