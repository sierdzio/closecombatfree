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
import "../../qml/gui"
import "../../qml/gui/menuEntries"

/*
  This file is in a sort of temporary state - everything is thrown in, just to
  test other functionality. For any real release, the whole game menu, including
  this ScenarioMenu, should be redesigned.
  */
Rectangle {
    property color backgroundColor: "#bb333232"

    signal scenarioEntryClicked (string scenarioPath)
    onScenarioEntryClicked: {
        gameLoader.load(scenarioPath, false);
    }

    id: root
    color: backgroundColor
    border.color: Qt.tint(backgroundColor, "#88aaaaaa")
    border.width: 1
    radius: 20

    Component.onCompleted: {
        var list = GameManager.scenarioList();
        for (var i = 0; i < list.length; ++i) {
            scenarioModel.append({"scenarioText": list[i]});
        }

        scenarios.currentIndex = 0;
        scenarios.height = scenarioModel.count * scenarios.currentItem.height;
        scenarios.width = scenarioModel.count * scenarios.currentItem.width;
        scenarios.currentIndex = -1;
    }

    ListModel {
        id: scenarioModel
    }

    Component {
        id: scenarioDelegate

        ScenarioMenuEntry {
            text: scenarioText
            anchors.horizontalCenter: parent.horizontalCenter

            Component.onCompleted: {
                entryClicked.connect(scenarioEntryClicked);
            }
        }
    }

    ListView {
        id: scenarios
        anchors.fill: parent
        anchors.margins: 15
        height: 50
        spacing: 2

        model: scenarioModel
        delegate: scenarioDelegate
    }
}
