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
    property string entryText: ""
    property string entryStatusText: ""
    property color entryStatusColor: "#000000"
    property string entryLogo: ""
    property color backgroundColor: "#7e8c24"
    property bool mouseAreaEnabled: true
    property bool selected: false
    property int index: 0

    property int fixedWidth: -1
    property int fixedHeight: -1

    id: root
    color: backgroundColor
    border.color: "#1e1c00"
    border.width: 1

    width: {
        if (fixedWidth == -1) {
            var textWidth = entryStatus.paintedWidth;

            if (entryDescription.paintedWidth > textWidth) {
                textWidth = entryDescription.paintedWidth;
            }

            return logo.width + textWidth + 5;
        } else {
            return fixedWidth;
        }
    }

    height: {
        if (fixedHeight == -1) {
            if ((entryDescription.paintedHeight + entryStatus.paintedHeight) > (logo.height + 5)) {
                return (entryDescription.paintedHeight + entryStatus.paintedHeight);
            } else {
                return logo.height + 3;
            }
        } else {
            return fixedHeight;
        }
    }

    onEntryStatusTextChanged: {
        if (entryStatusText == "") {
            return;
        }

        var newColor = EngineHelpers.colorForStatus(entryStatusText);
        if (newColor !== "ERROR")
            entryStatusColor = newColor;
        else
            console.log("Error while changing status message color!");
    }

    onSelectedChanged: {
        if (selected == true) {
            root.border.color = "#eeee11";;
        } else if (selected == false) {
            root.border.color = "#1e1c00";
        }
    }

    Image {
        id: logo
        height: 50
        width: 50
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 2
        anchors.topMargin: 2
        source: entryLogo
    }

    Text {
        id: entryDescription
        anchors.left: logo.right
        anchors.top: parent.top
        anchors.right: parent.right
        color: "#000000"
        text: entryText
        font.pointSize: 9
        font.bold: true
        font.family: "Ubuntu Condensed"
        verticalAlignment: Text.Top
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: entryStatus
        anchors.left: logo.right
        anchors.top: entryDescription.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: entryStatusColor
        text: entryStatusText
        font.pointSize: 12
        style: Text.Raised
        font.bold: true
        font.family: "Ubuntu Condensed"
        verticalAlignment: Text.Top
        horizontalAlignment: Text.AlignHCenter
    }

    MouseArea {
        id: moveFastMouseArea
        enabled: mouseAreaEnabled
        anchors.fill: parent
        hoverEnabled: mouseAreaEnabled

        onEntered: parent.color = Qt.tint(backgroundColor, "#80202020");
        onExited: parent.color = backgroundColor;
    }
}
