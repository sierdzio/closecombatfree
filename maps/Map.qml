import QtQuick 1.1
import "../units/units.js" as Units

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

    function setUnits(newList) {
        Units.list = newList;

        // Experimental event searching.
        // Would be better to optimise this.
        // For example, add coarse filter in Unit.qml
        // that would send a signal only once 5-10 units,
        // and would combine X and Y signals into one.
        for (var i = 0; i < Units.list.length; i++) {
            var unit = Units.list[i];
            unit.positionChanged.connect(checkForHits);
//            console.log("Unit number " + i + " is set in Map element.");
        }
    }

    function checkForHits(x, y, index) {
        var child = childAt(x, y);
        if ((child == null) || (child == background) || (child == hipsometricMap))
            return;

        // WARNING! This checks x and y only. No check for width/height/rotation. Yet.

        console.log("Hit! Who: " + child);
        child.removeTop();
    }
}
