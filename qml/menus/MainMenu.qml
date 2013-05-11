import QtQuick 2.0
import "../../qml/gui"
import "../../qml/gui/menuEntries"
import "../../qml/campaigns"
import "../../qml/scenarios"

Rectangle {
    id: root
    color: "#aa228844"

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
        }

        CardMenuEntry {
            text: "Campaigns:"
        }

        CardMenuEntry {
            text: "Saved games:"
        }
    }

    ScenarioMenuEntry {
        id: quitButton
        text: "Quit"
        anchors.bottom: parent.bottom
        anchors.margins: 20
        anchors.horizontalCenter: parent.horizontalCenter

        onEntryClicked: Qt.quit()
    }
}
