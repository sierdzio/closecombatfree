import QtQuick 1.1

Item {
    property string cover: "none"
    property bool topVisible: true

    function removeTop() {
        if (topVisible == true) {
            topVisible = false;
        }
    }
    function toggleTop() {
        topVisible = !topVisible;
    }

    // Warning! All props should have "objectName" set!
    // This information is used in terrain info.
    objectName: "default prop"

//    MouseArea {
//        anchors.fill: parent

//        onClicked: {
//            toggleTop();
//        }
//    }
}
