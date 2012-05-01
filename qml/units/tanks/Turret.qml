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

Item {
    property int centerX: turretSize/2
    property int centerY: turretSize/2
    property string bodyTexture: ""
    property bool firing: false
    property bool smoking: false
    property color turretColor: "#7b8259"
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

    Rectangle {
        id: barrelBase
        width: 5
        height: 30
        color: turretColor
        border.width: 1
        border.color: "#4f5e1f"
        anchors.bottom: body.top
        anchors.bottomMargin: -1
        anchors.horizontalCenter: body.horizontalCenter

        Rectangle {
            id: barrelSegment1 // Some tanks have 2 recoil segments.
            width: 3
            height: 42
            color: turretColor
            border.width: 1
            border.color: "#4f5e1f"
            anchors.bottom: barrelBase.top
            anchors.bottomMargin: 0
            anchors.horizontalCenter: barrelBase.horizontalCenter
            z: barrelBase.z - 1

            Image {
                property int imageNumber: 0
                id: fireImage
                visible: true
                source: ""
                scale: 2
                anchors.bottom: barrelSegment1.top
                anchors.horizontalCenter: barrelSegment1.horizontalCenter
            }
        }
    }

    Timer {
        id: fireTimer
        interval: 20
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            var imgNumber = fireImage.imageNumber;

            if (imgNumber != 5) {
                fireImage.imageNumber = imgNumber + 1;
                fireImage.source = "../../../img/effects/gun_fire" + (imgNumber + 1) + ".png";
            } else if (imgNumber == 5) {
                fireImage.imageNumber = 0;
                fireImage.source = "";
                fireTimer.stop();
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
                        fireTimer.start();
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
