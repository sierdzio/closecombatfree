import QtQuick 1.1
import "qrc:/skin/"

Item {
    id: root
    width: 100
    height: 62

    Component.onCompleted: {
        // Has to be redone every time this window opens!
        var list = savedGamesList();
        for (var i = 0; i < list.length; i++) {
            var current = list[i];
            gameModel.append({"gameText": current});
        }

        games.currentIndex = 0;
        games.height = gameModel.count * games.currentItem.height;
        games.currentIndex = -1;
    }

    signal entryClicked (string gamePath)

    ListModel {
        id: gameModel
    }

    Component {
        id: gameDelegate

        ScenarioMenuEntry {
            text: gameText
            anchors.horizontalCenter: parent.horizontalCenter

            Component.onCompleted: {
                entryClicked.connect(root.entryClicked);
            }
        }
    }

    ListView {
        id: games
        anchors.fill: parent

        model: gameModel
        delegate: gameDelegate
    }
}
