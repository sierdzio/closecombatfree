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
import "../../qml/gui/menuEntries"

Rectangle {
    property int entryWidth: 175
    property bool empty: true
    property color backgroundColor: "#7e8c24"

    id: root
    height: 200
    width: 180
    color: backgroundColor
    border.color: "#1e1c00"
    border.width: 2

    Component.onCompleted: {
        Logger.newStatusMessage.connect(newStatusMessageReceived);
    }

    function newStatusMessageReceived(message, object) {
        messageModel.append({"message": message});
        if (empty == true)
            empty = false;
    }

    ListModel {
        id: messageModel
    }

    ListView {
        id: messages
        anchors.fill: parent
        anchors.topMargin: 2
        anchors.leftMargin: 2
        spacing: 2
//        clip: true

            model: messageModel

            delegate: StatusMessageMenuEntry {
                width: entryWidth
                entryText: message
                backgroundColor: root.backgroundColor
            }
    }
}
