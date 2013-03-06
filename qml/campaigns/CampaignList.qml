import QtQuick 2.0
import "../../qml/gui"

ListView {
    id: root
    width: 100
    height: 250

    delegate: ScenarioMenuEntry {
        text: GameManager.campaignPath(index);
    }

    model: GameManager.campaignList();
}
