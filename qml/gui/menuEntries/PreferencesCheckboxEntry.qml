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

Text {
    property bool checked: true
    property bool enabled: true

    id: mainText
    color: "#adadad"
    text: "Entry text"
    width: paintedWidth  + 10 + checkBox.width
    height: paintedHeight
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft
    font.pointSize: 8
    font.bold: true

    Rectangle {
        id: checkBox
        color: "#00000000"
        border.width: 1
        border.color: "#adadad"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        height: 15
        width: 15
        radius: 5

        Image {
            id: tickImage
            anchors.fill: parent
            source: "../../../img/gui/checkBoxTick.png"
            visible: checked
        }
    }

    MouseArea {
        id: mouseAreaEntry
        acceptedButtons: Qt.LeftButton
        anchors.fill: parent
        enabled: mainText.enabled

        onClicked: {
            checked = !checked;
        }
    }
}
