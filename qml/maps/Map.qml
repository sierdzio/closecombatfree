/****************************************************************************
** Close Combat Free is a free, easily moddable CC-like game.
** Copyright (C) 2011 Tomasz Siekierda
** Email: sierdzio@gmail.com, website: http://www.sierdzio.com
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program (in doc/ directory).
** If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
****************************************************************************/

import QtQuick 2.0
import QmlBase 0.1

BaseMap {
    property variant units

    id: root
    width: background.sourceSize.width
    height: background.sourceSize.height

    Image {
        id: hipsometricMap
        source: hipsometricImage
        anchors.fill: parent
        z: -1

        Component.onCompleted: {
            Terrain.setTerrainImageUrl(hipsometricImage, hipsometricMap.width, hipsometricMap.height);
        }
    }

    Image {
        id: background
        source: backgroundImage
        anchors.fill: parent
        z: 0
    }

    function setUnits(units) {
        for (var i = 0; i < units.length; i++) {
            if (units[i].objectType !== undefined && units[i].objectType === "unit")
                units[i].positionChanged.connect(checkForHits);
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
        if ((child === null) || (child === background) || (child === hipsometricMap))
            return false;
        else
            return true;
    }

    function getProps() {
        var result = new Array(children.length - 2);

        for (var i = 0; i < children.length - 2; ++i) {
            result[i] = (children[i + 2]);
        }

        return result;
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

        var pixelInfo = Terrain.pixelInfo(x, y);
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
