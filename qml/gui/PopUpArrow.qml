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
    property int size: 50
    property bool poppedUp: false
    property color backgroundColor: "#dd333232"

    color: backgroundColor
    border.color: Qt.tint(backgroundColor, "#88aaaaaa")
    border.width: 1
    width: size
    height: width
    radius: 5

    Image {
        opacity: 0.7
        width: parent.width - 10
        height: width
        source: "../../img/gui/popUpArrow.png"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        rotation: (poppedUp)? 180 : 0
    }

    MouseArea {
        id: mouseAreaPopUp
        acceptedButtons: Qt.LeftButton
        anchors.fill: parent

        onClicked: {
            poppedUp = !poppedUp;
        }
    }
}
