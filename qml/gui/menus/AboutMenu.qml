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
import "../../../qml/gui/menuEntries"

Rectangle {
    property color backgroundColor: "#bb333232"
    property string aboutCcfText: Global.getFileContents("README");
    property string aboutQtText: "About Qt body"
    property string licenseText: Global.getFileContents("doc/LICENSE.txt");

    id: root
    color: backgroundColor
    border.color: Qt.tint(backgroundColor, "#88aaaaaa")
    border.width: 1

    width: Config.windowWidth
    height: Config.windowHeight
    radius: 20

    function toggleVisibility() {
        root.visible = !root.visible;
    }

    MouseArea {
        id: blockingArea
        anchors.fill: parent
    }

    // Tabs
    Row {
        id: tabHeaders

        spacing: 5
        anchors.top: parent.top
        anchors.topMargin: parent.radius
        anchors.horizontalCenter: parent.horizontalCenter

        MenuEntry {
            id: aboutCcfTab
            text: "About CCF"

            onEntryClicked: tabText.text = aboutCcfText;
        }

        MenuEntry {
            id: aboutQtTab
            text: "About Qt"

            onEntryClicked: tabText.text = aboutQtText;
        }

        MenuEntry {
            id: licenseTab
            text: "License"

            onEntryClicked: tabText.text = licenseText;
        }
    }

    // Temp, experimental. Will be changed to something more flexible later.
    // Also, docs might be added here, on a web kit view.
    Flickable {
        flickableDirection: Flickable.VerticalFlick
        contentWidth: tabText.width
        contentHeight: tabText.height

        anchors.top: tabHeaders.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: closeButton.top
        anchors.leftMargin: parent.radius

        Text {
            id: tabText

            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width // - root.radius
            height: paintedHeight

            color: "#adadad"
            wrapMode: Text.WordWrap
            text: aboutCcfText
        }
    }

    // Bottom button
    MenuEntry {
        id: closeButton

        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: parent.radius
        anchors.rightMargin: parent.radius


        text: "Close"
        additionalText: "This will close About menu."

        onEntryClicked: toggleVisibility();
    }
}
