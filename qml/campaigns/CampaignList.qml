import QtQuick 2.1
import "../../qml/gui/menuEntries"

ListView {
    id: root
    width: 100
    height: 250
    signal entryClicked (string text)

    delegate: ScenarioMenuEntry {
        text: GameManager.campaignPath(index);

        Component.onCompleted: {
            entryClicked.connect(root.entryClicked);
        }
    }

    model: GameManager.campaignList();
}
