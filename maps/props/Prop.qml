import QtQuick 1.1

Item {
    property bool topVisible: true

    function removeTop() {
        if (top.visible) {
            topVisible = false;
        }
    }
    function toggleTop() {
        topVisible = !topVisible;
    }
}
