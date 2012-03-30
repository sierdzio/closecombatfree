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

import QtQuick 1.1
import "../../qml/gui"
import "../../qml/gui/menuEntries"

/*
  This file is in a sort of temporary state - everything is thrown in, just to
  test other functionality. For any real release, whe whole game menu, including
  this ScenarioMenu, should be redesigned.
  */
Rectangle {
    property string __tempReloadPath: ""

    signal scenarioEntryClicked (string scenarioPath)
    onScenarioEntryClicked: {
        scenario.scenarioFile = "scenarios/" + scenarioPath;
        scenario.source = "../../qml/scenarios/Scenario.qml";
        root.state = "opened";
    }

    signal campaignEntryClicked (string campaignPath)
    onCampaignEntryClicked: {
        // Damn, what now?
        // Most probably, a new QML thing has to go here:
        // CampaignManager.qml, or something that would take over the control,
        // and ensure that campaigns work as expected.
        // For now, a dirty hack will probably be used.
        scenario.scenarioFile = "campaigns/" + campaignPath;
        scenario.source = "../../qml/scenarios/Scenario.qml";
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
        var list = GameManager.scenariosList();
        for (var i = 0; i < list.length; i++) {
            var current = list[i];
            scenarioModel.append({"scenarioText": current});
        }

        var campaignList = GameManager.qmlFileList("campaigns");
        for (var i = 0; i < campaignList.length; i++) {
            var current = campaignList[i];
            campaignModel.append({"campaignText": current});
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
        id: campaignModel
    }

    Component {
        id: campaignDelegate

        RosterMenuEntry {
            entryText: camapignText
            entryStatusText: campaignDescription
            entryStatusColor: "#000000"
            entryLogo: campaignLogo
            backgroundColor: "#7e8c24"
            mouseAreaEnabled: false
            selected: false
            anchors.horizontalCenter: parent.horizontalCenter

            Component.onCompleted: {
                entryClicked.connect(campaignEntryClicked);
            }
        }
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
            id: campaigns
            height: 300
            width: 400
            spacing: 2

            model: campaignModel
            delegate: campaignDelegate
        }

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
                        scenario.source = "../../qml/scenarios/Scenario.qml";
                        root.state = "opened";
                    }
                }
            }
        }
    ]
}
