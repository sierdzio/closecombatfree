// Currently, this file is not being used!
import QtQuick 1.1

Item {
    property string mapFile: ""

    id: root
    width: map.width
    height: map.height

    Loader {
        id: map
        source: mapFile
        anchors.fill: parent
    }
}
