import QtQuick 1.1

Prop {
    width: 140
    height: 100

    Rectangle {
        id: base
        anchors.fill: parent
        color: "#999fa4"

        Rectangle {
            id: floor
            anchors.fill: parent
            anchors.margins: 5
            color: "#4f1708"
        }
    }

    Rectangle {
        id: roof
        visible: topVisible
        anchors.fill: parent
        color: "#8899ff"
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            toggleTop();
        }
    }
}
