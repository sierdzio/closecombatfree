import QtQuick 1.1

Rectangle {
    property int centerX: width/2
    property int centerY: height/2
    property color orderColor: "#bb3333"
    property int index: -1
    property int number: -1

    signal dragComplete(int index, int number)

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

        Text {
            text: (number == -1)? "" : number + 1;
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            style: Text.Outline
            anchors.fill: parent
            anchors.topMargin: 1
            anchors.leftMargin: 1
            font.pointSize: 7
            color: "#dddddd"
        }
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
                    dragComplete(index, number);
                }
            }
        }
    }
}
