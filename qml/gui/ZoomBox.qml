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

import QtQuick 1.1

Rectangle {
    property int size: 50
    property int currentZoom: 100
    property color backgroundColor: "#bb333232"

    signal zoomIn();
    signal zoomOut();

    color: backgroundColor
    border.color: Qt.tint(backgroundColor, "#88aaaaaa")
    border.width: 1
    width: (2 * height) + zoomText.paintedWidth
    height: size
    radius: 5

    Component.onCompleted: {
        zoomInButton.zoomButtonClicked.connect(zoomIn);
        zoomOutButton.zoomButtonClicked.connect(zoomOut);
    }

    ZoomButton {
        id: zoomInButton
        text: "+"
        size: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
    }

    Text {
        id: zoomText
        text: currentZoom + "%" //(currentZoom * 100) + "%"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: size/3
        width: 40
        color: "#ffffff"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    ZoomButton {
        id: zoomOutButton
        text: "-"
        size: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
    }
}
