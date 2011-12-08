import QtQuick 1.1

Item {
    // Warning! All props should have "objectName" set!
    // This information is used in terrain info.
    objectName: "default prop"
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

//    MouseArea {
//        anchors.fill: parent

//        onClicked: {
//            toggleTop();
//        }
//    }
}
