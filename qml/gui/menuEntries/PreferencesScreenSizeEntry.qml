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

Text {
    property bool enabled: true

    id: mainText
    color: "#adadad"
    text: "Window size"
    width: paintedWidth  + row.width
    height: paintedHeight
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft
    font.pointSize: 8
    font.bold: true

    function getHeight() {
        return inputHeight.text;
    }

    function getWidth() {
        return inputWidth.text;
    }

    function setHeight(newHeight) {
        inputHeight.text = newHeight;
    }

    function setWidth(newWidth) {
        inputWidth.text = newWidth;
    }

    Row {
        id: row
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        spacing: 3

        Rectangle {
            id: widthEdit
            color: "#00000000"
            border.width: 1
            border.color: "#adadad"
            height: 15
            width: 50
            radius: 5

            TextInput {
                id: inputWidth
                color: "#adadad"
                anchors.fill: parent
                anchors.margins: 2
                enabled: mainText.enabled

                horizontalAlignment: Text.AlignRight
                font.pointSize: 8
            }
        }

        Text {
            id: xSign
            color: "#adadad"
            text: "x"
            width: paintedWidth
            height: paintedHeight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 8
            font.bold: true
        }

        Rectangle {
            id: heightEdit
            color: "#00000000"
            border.width: 1
            border.color: "#adadad"
            height: 15
            width: 50
            radius: 5

            TextInput {
                id: inputHeight
                color: "#adadad"
                anchors.fill: parent
                anchors.margins: 2
                enabled: mainText.enabled

                horizontalAlignment: Text.AlignRight
                font.pointSize: 8
            }
        }

        Text {
            id: pixelsSign
            color: "#adadad"
            text: "pixels"
            width: paintedWidth
            height: paintedHeight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 8
            font.bold: true
        }
    }
}
