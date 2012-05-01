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
    property string headerText: "Header"
    property string bodyText: "Some description"

    id: root
    width: ((header.paintedWidth < body.paintedWidth)? body.paintedWidth : (header.paintedWidth) + 6);
    height: header.paintedHeight + body.paintedHeight + 4;
    color: "#bb333232"
    radius: 10

    Text {
        id: header
        text: headerText
        color: "#d6d6a1"
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 14

        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: body
            text: bodyText
            color: "#ffffff"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 9

            anchors.top: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
