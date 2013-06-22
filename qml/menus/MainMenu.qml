import QtQuick 2.1
import "../../qml/gui"
import "../../qml/gui/menuEntries"
import "../../qml/campaigns"
import "../../qml/scenarios"

Rectangle {
    id: root
    color: "#aa228844"
    //state: "default"

    Text {
        id: notice
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        text: "For now, player is assumed to be on \"neutral\" side.\nSide choosing will be added later."
        font.pointSize: 10
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Row {
        anchors.fill: parent
        anchors.margins: 45
        spacing: 45

        CardMenuEntry {
            text: "Scenarios:"
            onClicked: {
                scenarioMenu.visible = !scenarioMenu.visible
                closeButton.visible = true;
            }
        }

        CardMenuEntry {
            text: "Campaigns:"
            onClicked: {
                campaignMenu.visible = !campaignMenu.visible
                closeButton.visible = true;
            }
        }

        CardMenuEntry {
            text: "Saved games:"
            onClicked: {
                loadGameMenu.visible = !loadGameMenu.visible
                closeButton.visible = true;
            }
        }
    }

    ScenarioMenu {
        id: scenarioMenu
        anchors.fill: parent
        visible: false
    }

    CampaignMenu {
        id: campaignMenu
        anchors.fill: parent
        visible: false
    }

    LoadGameMenu {
        id: loadGameMenu
        anchors.fill: parent
        visible: false
    }

    ScenarioMenuEntry {
        id: closeButton
        text: "Close"
        anchors.bottom: parent.bottom
        anchors.margins: 20
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false

        onEntryClicked: {
            scenarioMenu.visible = false;
            campaignMenu.visible = false;
            loadGameMenu.visible = false;
            visible = false;
        }

        ScenarioMenuEntry {

            id: quitButton
            text: "Quit"
            anchors.bottom: parent.bottom
            anchors.left: parent.right
            anchors.leftMargin: 15

            onEntryClicked: Qt.quit()
        }
    }

    Loader {
        property string gameFile: ""

        id: gameLoader
        anchors.fill: parent
        visible: false

        onLoaded: {
            item.scenarioFile = gameFile;
            item.closeScenario.connect(close);
            item.loadScenario.connect(loadSaveGame)
            focus = true;
            visible = true;
        }

        function loadSaveGame(whatToLoad) {
            load(whatToLoad, true)
        }

        function load(whatToLoad, isSave) {
            close();
            if (isSave)
                gameFile = "saves/" + whatToLoad;
            else
                gameFile = "scenarios/" + whatToLoad;

            source = "../../qml/scenarios/Scenario.qml"
        }

        function close() {
            source = "";
            visible = false;
            focus = false;
        }
    }
}
