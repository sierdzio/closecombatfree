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

Rectangle {
    property int centerX: width/2
    property int centerY: height/2
    property color orderColor: "#bb3333"
    property int index: -1
    property int number: -1

    signal dragComplete(int index, int number)

    id: root
    width: 21
    height: width
    radius: width/2
    color: "#345634"
    border.color: "#111111"
    border.width: 1

    Rectangle {
        width: parent.width/2
        height: width
        radius: width/2
        color: orderColor

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Text {
            text: (number == -1)? "" : number + 1;
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            style: Text.Outline
            anchors.fill: parent
            anchors.topMargin: 1
            anchors.leftMargin: 1
            font.pointSize: 7
            color: "#dddddd"
        }
    }

    MouseArea {
        anchors.fill: parent

        drag {
            target: root
            axis: Drag.XandYAxis
            minimumX: 5
            minimumY: 5

            onActiveChanged: {
                if(drag.active == false) {
                    dragComplete(index, number);
                }
            }
        }
    }
}
