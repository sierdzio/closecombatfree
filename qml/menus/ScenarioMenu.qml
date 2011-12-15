import QtQuick 1.1
import "qrc:/core/scenarios"

Rectangle {
    signal entryClicked(string scenarioPath)
    onEntryClicked: {
        scenario.scenarioFile = "qrc:/scenarios/" + scenarioPath;
        scenario.source = "qrc:/core/scenarios/Scenario.qml";
        root.state = "opened";
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
    }

    function closeScenario() {
        root.state = "closed";
    }

    ListModel {
        id: scenarioModel
    }

    Component {
        id: scenarioDelegate

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width: entryText.paintedWidth + 10
            height: entryText.paintedHeight + 10
            color: "#000000"
            border.color: "#a0a0a0"
            border.width: 1

            Text {
                id: entryText
                color: "#ffffff"
                text: scenarioText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 10
                font.bold: true
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    entryClicked(scenarioText);
                }
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
