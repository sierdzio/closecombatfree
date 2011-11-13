import QtQuick 1.1
import "scenarios"

Rectangle {
    id: root
    width: 800
    height: 800
    color: "lightgray"

    Scenario {
        id: scenario
        anchors.fill: parent
    }
}
