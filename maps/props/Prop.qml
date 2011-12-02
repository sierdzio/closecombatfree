import QtQuick 1.1

Item {
    property bool topVisible: true

    function removeTop() {
        if (topVisible == true) {
            topVisible = false;
        }
    }
    function toggleTop() {
        topVisible = !topVisible;
    }

    objectName: "props/Prop"

//    MouseArea {
//        anchors.fill: parent

//        onClicked: {
//            toggleTop();
//        }
//    }
}
