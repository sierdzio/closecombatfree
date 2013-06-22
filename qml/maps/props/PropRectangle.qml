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
    // Warning! All props should have "objectName" set!
    // This information is used in terrain info.
    objectName: "default prop rectangle"
    property string cover: "none"
    property bool topVisible: true

    function removeTop() {
        if (topVisible == true) {
            topVisible = false;
        }
    }
    function toggleTop() {
        topVisible = !topVisible;
    }

//    MouseArea {
//        anchors.fill: parent

//        onClicked: {
//            toggleTop();
//        }
//    }
}
