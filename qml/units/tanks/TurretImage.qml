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

import QtQuick 2.1
import "../../effects"

Item {
    property int centerX: turretSize/2
    property int centerY: turretSize/2
    property string bodyTexture: ""
    property string barrelBaseTexture: ""
    property string barrelSegment1Texture: ""
    property bool firing: false
    property bool smoking: false
    property int turretSize: 60
    property int turretRotation: 0

    id: root
    width: turretSize
    height: turretSize

    transform: Rotation {
        origin.x: root.centerX; origin.y: root.centerY; angle: turretRotation
    }

    Image {
        id: body
        width: turretSize
        height: turretSize
        source: bodyTexture
        sourceSize.height: turretSize
        sourceSize.width: turretSize
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    Image {
        id: barrelBase
        width: 5
        height: 30
        anchors.bottom: body.top
        anchors.bottomMargin: -1
        anchors.horizontalCenter: body.horizontalCenter

        source: barrelBaseTexture

        Image {
            id: barrelSegment1 // Some tanks have 2 recoil segments.
            width: 3
            height: 42
            anchors.bottom: barrelBase.top
            anchors.bottomMargin: 0
            anchors.horizontalCenter: barrelBase.horizontalCenter
            z: barrelBase.z - 1

            source: barrelSegment1Texture

            GunFireEffect {
                id: gunFire
                anchors.bottom: barrelSegment1.top
                anchors.horizontalCenter: barrelSegment1.horizontalCenter
            }
        }
    }

    states: [
        State {
            name: "firing"
            when: ((firing == true) || (smoking == true))
            PropertyChanges {
                target: barrelSegment1
                anchors.bottomMargin: -25
            }
        }
    ]

    transitions: [
        Transition {
            to: "firing"
            ParallelAnimation {
                ScriptAction {
                    script: {
                        gunFire.pulse(200);
                    }
                }
                SequentialAnimation {
                    NumberAnimation {
                        target: barrelSegment1
                        property: "anchors.bottomMargin"
                        duration: 40
                    }
                    NumberAnimation {
                        target: barrelSegment1
                        property: "anchors.bottomMargin"
                        to: 0
                        duration: 500
                        easing.type: Easing.InOutQuad
                    }
                    ScriptAction {
                        script: {
                            firing = false;
                            smoking = false;
                        }
                    }
                }
            }
        }
    ]
}
