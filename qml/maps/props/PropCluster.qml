import QtQuick 1.1

Rectangle {
    property int numberOfObjects: 0
    property string baseElement: ""
//    property bool allowVariation: false // this could be good to create
    // mixed clusters (e.g. a forest with many kinds of trees)

    id: root
    width: 300
    height: 262

    color: "#00000000"
    border.width: 1
    border.color: "#ffff00"

    Repeater {
        anchors.fill: parent
        model: numberOfObjects

        Loader {
            x: {
                return Math.random(0) * root.width;
            }

            y: {
                return Math.random(0) * root.height;
            }

            source: baseElement
        }
    }
}
