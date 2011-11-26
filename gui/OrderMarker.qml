import QtQuick 1.1

Rectangle {
    property int centerX: width/2
    property int centerY: height/2
    property color orderColor: "#bb3333"
    property int index: -1

    signal dragComplete(int index)

    id: root
    width: 21
    height: width
    radius: width/2
    color: "#345634"
    border.color: "#111111"
    border.width: 1

    Rectangle {
        width: parent.width/2
        height: width
        radius: width/2
        color: orderColor

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea {
        anchors.fill: parent

        drag {
            target: root
            axis: Drag.XandYAxis
            minimumX: 5
            minimumY: 5

            onActiveChanged: {
                if(drag.active == false) {
                    dragComplete(index);
//                    console.log("Drag is now: " + drag.active);
                }
            }
        }
    }
}
