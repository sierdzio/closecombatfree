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

Rectangle {
    property int size: 30
    property string text: "o"
    property string additionalText: ""
    property color backgroundColor: "#bb333232"
    property bool enabled: true

    signal entryClicked()

    id: root
    color: backgroundColor
    border.color: Qt.tint(backgroundColor, "#88aaaaaa")
    border.width: 1

    width: {
        var result = 0;
        var textWidth = mainText.paintedWidth;
        if ((additional.visible == true) && (additional.paintedWidth > textWidth)) {
            textWidth = additional.paintedWidth;
        }

        if (size > textWidth)
            result = size;
        else
            result = textWidth;

        return (result + (radius));
    }

    height: {
        var objectsHeight = mainText.paintedHeight + additional.paintedHeight;
        if (objectsHeight > 30) {
            return objectsHeight;
        } else {
            return 30;
        }
    }

    function getContentsHeight() {
        var result = 0;
        var textHeight = mainText.paintedHeight;
        if (additional.visible == true) {
            textHeight += additional.paintedHeight;
        }

        result = textHeight;

        return (result + (radius));
    }

    radius: size/8

    Text {
        id: mainText
        color: "#adadad"
        text: parent.text
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 8
        font.bold: true
    }

    Text {
        id: additional
        color: "#adadad"
        text: parent.additionalText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: mainText.bottom
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 6
        visible: (parent.additionalText != "")? true : false;
    }

    MouseArea {
        id: mouseAreaEntry
        acceptedButtons: Qt.LeftButton
        anchors.fill: parent
        enabled: parent.enabled

        onClicked: {
            entryClicked();
        }
    }
}
