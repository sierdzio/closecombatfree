import QtQuick 1.1

Item {
    property int numberOfObjects: 0
    property string baseElement: ""
//    property bool allowVariation: false // this could be good to create
    // mixed clusters (e.g. a forest with many kinds of trees)

    id: root
    width: 300
    height: 262

    function getProps() {
        var result = new Array();

        for (var i = 0; i < repeater.count; ++i) {
            result.push(repeater.itemAt(i));
//            console.log(result[i].item.objectName + ", " + result[i].x + ", " + result[i].y);
        }

        return result;
    }

    Repeater {
        id: repeater
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
