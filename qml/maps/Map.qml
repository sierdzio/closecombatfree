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
    // Defines global opacity value.
    // Useful for temporarily revealing hidden props
    // (like, water objects etc.)
    property real propOpacity: 0.5

    property variant units

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

        Component.onCompleted: {
//            console.log("Sending terrain info to C++. Image URL: " + hipsometricImage
//                        + " . Width: " + hipsometricMap.width
//                        + " . Height: " + hipsometricMap.height);
            setTerrainImageUrl(hipsometricImage, hipsometricMap.width, hipsometricMap.height);
        }
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

    function setUnits(newList) {
        units = newList;
        for (var i = 0; i < units.length; i++) {
            var unit = units[i];
            unit.positionChanged.connect(checkForHits);
        }
    }

    function checkForHits(x, y, index) {
        if (!childExistsAt(x, y))
            return;

        var child = childAt(x, y);
        // WARNING! This checks x and y only. No check for width/height/rotation. Yet.

        console.log("Hit! Who: " + child);
        if (child.topVisible !== undefined)
            child.removeTop();
    }

    function childExistsAt(x, y) {
        var child = childAt(x, y);
        if ((child == null) || (child == background) || (child == hipsometricMap))
            return false;
        else
            return true;
    }

    function terrainInfo(x, y) {
        // This method is intended to extract terrain information on a given point.
        // This info should include: type of terrain/ obstacle/ prop, height over "0" level,
        // how much cover does a given spot give to a unit.
        var result = {objectType: "unknown", heightOverZero: 5, cover: "poor"};

        if (childExistsAt(x, y)) {
            var child = childAt(x, y);
            result.objectType = child.objectName;
            result.cover = child.cover;
        }

        var pixelInfo = terrainPixelInfo(x, y);
//        console.log("Got terrain PIXEL info: " + pixelInfo);
        result.heightOverZero = pixelInfo / 10;

        return result;
    }

    function terrainInfoString(x, y) {
        var info = terrainInfo(x, y);
        var result = "Terrain: " + info.objectType
            + ". Height: " + info.heightOverZero + " meters"
            + ". Cover: " + info.cover;
        return result;
    }
}
