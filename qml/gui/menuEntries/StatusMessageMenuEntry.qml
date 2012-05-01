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
import "../../../qml/engineLogicHelpers.js" as Logic

Rectangle {
    property string entryText: ""
    property color entryTextColor: "#000000"
    property color backgroundColor: "#7e8c24"
    property string entryPicture: ""

    id: root
    color: backgroundColor
    border.color: "#1e1c00"
    border.width: 1
//    width: entry.paintedWidth
//    height: entry.paintedHeight
    width: {
        return logo.width + entry.paintedWidth;
    }

    height: {
        if (entry.paintedHeight > logo.height) {
            return entry.paintedHeight;
        } else {
            return logo.height;
        }
    }

    Text {
        id: entry
        height: paintedHeight
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: logo.left
        color: entryTextColor
        text: entryText
        font.pointSize: 9
        font.family: "Ubuntu Condensed"
        verticalAlignment: Text.Top
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
    }

    Image {
        id: logo
        height: entry.height
        width: entry.height
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.leftMargin: 2
        anchors.topMargin: 2
        source: entryPicture
    }
}
