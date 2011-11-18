import QtQuick 1.1

Hull {
    property string hullFile: ""

    Loader {
        id: hull
        source: hullFile
        anchors.fill: parent

        onLoaded: {
            item.hullColor = hullColor;
            item.hullWidth = hullWidth;
            item.hullHeight = hullHeight;
            centerX = item.centerX;
            centerY = item.centerY;
        }
    }
}
