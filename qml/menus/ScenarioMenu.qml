import QtQuick 1.1
import "qrc:/skin"

Rectangle {
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
    width: 100
    height: 100
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
        scenarios.currentIndex = -1;
        quitButton.entryClicked.connect(quitEntryClicked);
    }

    function closeScenario() {
        root.state = "closed";
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

    ListView {
        id: scenarios
        height: 50
        spacing: 2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        model: scenarioModel
        delegate: scenarioDelegate
    }

    ScenarioMenuEntry {
        id: quitButton
        text: "Quit"
        anchors.top: scenarios.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: scenarios.horizontalCenter
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
        }
    ]

    transitions: [
        Transition {
            from: "closed"
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
        }
    ]
}
