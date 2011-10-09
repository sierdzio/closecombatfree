import QtQuick 1.0

Item {
    property string imageSource: "../img/gui/defenceSphere_blue.png"

    id: root
    width: 120
    height: 100

    Image {
        anchors.right: parent.right
        source: imageSource
    }
}
