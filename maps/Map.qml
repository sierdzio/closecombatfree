import QtQuick 1.1

Item {
    // Stores background image. Preferably in PNG, although all
    // QML formats are supported.
    property string backgroundImage: ""
    // Holds path to hipsometric map of the terrain.
    // Terrain defined by intensity of color in the image.
    // Should be the same size as backgroundImage. If it's not,
    // it will be stretched to match background.
    property string hipsometricImage: ""
    // Defines, whether hipsometric map should be visible
    property bool hipsometricMapInFront: false

    onHipsometricMapInFrontChanged: {
        swapBackgrounds();
    }

    id: root
    width: background.sourceSize.width
    height: background.sourceSize.height

    Image {
        id: hipsometricMap
        source: hipsometricImage
        anchors.fill: parent
        z: -1
    }

    Image {
        id: background
        source: backgroundImage
        anchors.fill: parent
        z: 0
    }

    function swapBackgrounds() {
        var z1 = hipsometricMap.z
        var z2 = background.z

        hipsometricMap.z = z2;
        background.z = z1;
    }
}
