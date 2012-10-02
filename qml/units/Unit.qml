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

import "../../qml/gui"
import "../../qml/units"

BaseUnit {
    centerX: unitWidth/2
    centerY: unitHeight/2

    id: root
    width: unitWidth
    height: unitHeight
    state: "healthy"

    signal positionChanged(real x, real y, int index)

    onXChanged: {
        if (Math.round(x % 10) === 0) {
            positionChanged(x, y, unitIndex);
        }
    }

    onYChanged: {
        if (Math.round(y % 10) === 0) {
            positionChanged(x, y, unitIndex);
        }
    }

    Text {
        id: groupMembership
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        z: 5

        height: 10
        width: 10
        color: "#ffffff"
        text: (groupNumber == 0) ? "" : groupNumber
        font.pointSize: 10
        style: Text.Raised
        font.bold: true
        font.family: "Ubuntu Condensed"
        verticalAlignment: Text.Bottom
        horizontalAlignment: Text.AlignRight
        rotation: -root.rotation
    }

    UnitSelectionBox {
        id: selectionBox
        unitWidth: parent.unitWidth
        unitHeight: parent.unitHeight
        visible: selected
        z: 3
    }

    DefenceSphere {
        id: defenceSphere
        z: hull.z - 1

        visible: {
            if (defenceSphereColor == "") {
                return false;
            } else {
                return true;
            }
        }
        imageSource: {
            if (defenceSphereColor == "")
                return "";
            else
                return "../../img/gui/defenceSphere_" + defenceSphereColor + ".png"

        }
        anchors {
            top: hull.top
            left: hull.left
            topMargin: 0
            leftMargin: centerX
        }

        transform: Rotation {
            origin.x: 0
            origin.y: defenceSphere.height/2
            angle: defenceSphereRotation
        }
    }

    Image {
        id: sideMarkImage
        x: 0
        y: 0
        z: 5
        width: 10
        height: 10
        rotation: -root.rotation
        source: sideMarkSource
        visible: sideMarkVisible
    }

    RotationAnimation on rotation {
        objectName: "rotationAnimation"
        id: rotationAnimation
        duration: 2000
        direction: RotationAnimation.Shortest
        easing.type: Easing.InOutQuad
        paused: running? root.paused : false

        onRunningChanged: { // Not sure whether doing that will be good under all circumstances
            if ((!rotationAnimation.running) && (unitStatus != "STOPPED")) {
                xMoveAnimation.to = orderTarget().x - centerX;
                xMoveAnimation.running = true;
                yMoveAnimation.to = orderTarget().y - centerY;
                yMoveAnimation.running = true;
            }
        }
    }

    NumberAnimation on x {
        objectName: "xMoveAnimation"
        id: xMoveAnimation
        duration: 2500
        easing.type: Easing.InOutQuad
        paused: running? root.paused : false

        onRunningChanged: {            
            if (!xMoveAnimation.running) {
                moving = false;
            }

            if ((!xMoveAnimation.running) && (unitStatus != "STOPPED")) {
                // Warning! This order is important for order markers!
                changeStatus("READY");
                actionFinished(unitIndex, orderTarget().x, orderTarget().y);
                continueQueue();
            } else if (unitStatus == "STOPPED") {
                changeStatus("READY");
            }
        }
    }

    NumberAnimation on y {
        objectName: "yMoveAnimation"
        id: yMoveAnimation
        duration: 2500
        easing.type: Easing.InOutQuad
        paused: running? root.paused : false
    }

    //// Temp debug
    Rectangle {
        id: destroyedState
        visible: (parent.state === "healthy")? false : true;
        width: 10
        height: width
        radius: 10
        color: "#ff3333"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        z: 10
    }
}
