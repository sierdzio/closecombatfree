import QtQuick 2.0
import "../../qml/campaigns"

Rectangle {
    property color backgroundColor: "#bb333232"

    id: root
    color: backgroundColor
    border.color: Qt.tint(backgroundColor, "#88aaaaaa")
    border.width: 1
    radius: 20

    signal scenarioEntryClicked (string campaignPath)
    onScenarioEntryClicked: {
        gameLoader.load(campaignPath, false);
    }

    CampaignList {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 15

        Component.onCompleted: {
            entryClicked.connect(scenarioEntryClicked);
        }

        CampaignDescriptionGui {
            anchors.left: parent.right
            anchors.top: parent.top
            anchors.margins: 15
        }
    }    
}
