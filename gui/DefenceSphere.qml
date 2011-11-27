import QtQuick 1.1

Item {
    property string imageSource: PWD + "img/gui/defenceSphere_blue.png"

    id: root
    width: 120
    height: 100

    Image {
        anchors.right: parent.right
        source: imageSource
    }
}