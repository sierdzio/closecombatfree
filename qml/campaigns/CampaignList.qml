import QtQuick 2.1
import "../../qml/gui/menuEntries"

ListView {
    id: root
    width: 100
    height: 250

    delegate: ScenarioMenuEntry {
        text: GameManager.campaignPath(index);
    }

    model: GameManager.campaignList();
}
