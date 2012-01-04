import QtQuick 1.1
import "qrc:/skin"
import "qrc:/skin/menuEntries"

Rectangle {
    property string __tempReloadPath: ""

    signal scenarioEntryClicked (string scenarioPath)
    onScenarioEntryClicked: {
        scenario.scenarioFile = "qrc:/scenarios/" + scenarioPath;
        scenario.source = "qrc:/core/scenarios/Scenario.qml";
        root.state = "opened";
    }
    signal quitEntryClicked (string ignoreThisString)
    onQuitEntryClicked: {
        Qt.quit();
    }

    id: root
    state: "closed"
    color: "#5f5f5f"

    Component.onCompleted: {
        var list = scenariosList();

        for (var i = 0; i < list.length; i++) {
            var current = list[i];
            scenarioModel.append({"scenarioText": current});
        }

        scenarios.currentIndex = 0;
        scenarios.height = scenarioModel.count * scenarios.currentItem.height;
        scenarios.width = scenarioModel.count * scenarios.currentItem.width;
        scenarios.currentIndex = -1;
        quitButton.entryClicked.connect(quitEntryClicked);
        entries.entryClicked.connect(loadGame);
    }

    function closeScenario() {
        root.state = "closed";
    }

    function loadGame(path) {
        __tempReloadPath = "saves/" + path;
        root.state = "loadingSavedGame";
    }

    ListModel {
        id: scenarioModel
    }

    Component {
        id: scenarioDelegate

        ScenarioMenuEntry {
            text: scenarioText
            anchors.horizontalCenter: parent.horizontalCenter

            Component.onCompleted: {
                entryClicked.connect(scenarioEntryClicked);
            }
        }
    }

    Row {
        id: entryBox
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        ListView {
            id: scenarios
            height: 50
            spacing: 2

            model: scenarioModel
            delegate: scenarioDelegate
        }

        LoadGameEntries {
            id: entries
            height: entryBox.height
        }
    }

    ScenarioMenuEntry {
        id: quitButton
        text: "Quit"
        anchors.top: entryBox.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: notice
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        text: "For now, player is assumed to be on \"neutral\" side.\nSide choosing will be added later."
        font.pointSize: 10
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Loader {
        property string scenarioFile: ""

        id: scenario
        width: root.width
        anchors.top: root.top
        anchors.bottom: root.bottom
        anchors.left: root.right

        onLoaded: {
            item.scenarioFile = scenarioFile;
            item.closeScenario.connect(root.closeScenario);
            item.loadScenario.connect(root.loadGame)
            focus = true;
        }
    }

    states: [
        State {
            name: "closed"
            AnchorChanges {
                target: scenario
                anchors.left: root.right
            }
        },

        State {
            name: "opened"
            AnchorChanges {
                target: scenario
                anchors.left: root.left
            }
        },

        State {
            name: "loadingSavedGame"
            AnchorChanges {
                target: scenario
                anchors.left: root.right
            }
        }
    ]

    transitions: [
        Transition {
            to: "opened"

            SequentialAnimation {
                ScriptAction {
                    script: {
                        scenario.visible = true;
                    }
                }
                AnchorAnimation {
                    duration: 500
                }
            }
        },

        Transition {
            from: "opened"
            to: "closed"

            SequentialAnimation {
                AnchorAnimation {
                    duration: 500
                }
                ScriptAction {
                    script: {
                        scenario.scenarioFile = "";
                        scenario.source = "";
                        scenario.visible = false;
                    }
                }
            }
        },

        Transition {
            to: "loadingSavedGame"

            SequentialAnimation {
                AnchorAnimation {
                    duration: 500
                }
                ScriptAction {
                    script: {
                        scenario.scenarioFile = "";
                        scenario.source = "";
                        scenario.visible = false;
                        scenario.scenarioFile = __tempReloadPath;
                        scenario.source = "qrc:/core/scenarios/Scenario.qml";
                        root.state = "opened";
                    }
                }
            }
        }
    ]
}
