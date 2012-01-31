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
import "../../../qml/gui/"

Item {
    id: root
    width: 100
    height: 62

    Component.onCompleted: {
        // Has to be redone every time this window opens!
        var list = GameManager.savedGamesList();
        for (var i = 0; i < list.length; i++) {
            var current = list[i];
            gameModel.append({"gameText": current});
        }

        games.currentIndex = 0;
        games.height = gameModel.count * games.currentItem.height;
        games.currentIndex = -1;
    }

    signal entryClicked (string gamePath)

    ListModel {
        id: gameModel
    }

    Component {
        id: gameDelegate

        ScenarioMenuEntry {
            text: gameText
            anchors.horizontalCenter: parent.horizontalCenter

            Component.onCompleted: {
                entryClicked.connect(root.entryClicked);
            }
        }
    }

    ListView {
        id: games
        anchors.fill: parent

        model: gameModel
        delegate: gameDelegate
    }
}
