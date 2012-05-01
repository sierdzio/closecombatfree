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
import QtQuick.Particles 2.0

import "../../../qml/units"
import "../../../qml/gui"
import "../../../qml/effects"
import "../../../qml/engineLogicHelpers.js" as Logic
import "../../../qml/units/engineActionLogic.js" as ActionLogic

Unit {
    // Tank properties.
    property int frontArmour: 100 // mm
    property int sideArmour: 80 // mm
    property int backArmour: 60 // mm
    property int turretArmour: 80 // mm - might evolve into {front, side, back} triplet, too

    property int mainGunReloadingTime: 5000

    // Additional properties, not important for non-developers/ modders.
    property int turretRotation: 0
    property int turretSize: 60
    property color hullColor: "#7b8259"

    onMoveTo: exhaust.pulse(2000);
    onMoveFastTo: exhaust.pulse(2000);
    onSneakTo: exhaust.pulse(2000);

    onTurretRotationChanged: turret.turretRotation = turretRotation;

    Component.onCompleted: {
        turret.turretSize = turretSize;
        hull.hullHeight = unitHeight;
        hull.hullWidth = unitWidth;
        hull.hullColor = hullColor;

        turret.x = centerX - turret.centerX;
        turret.y = centerY - turret.centerY;
    }

    signal fireTo (real targetX, real targetY)
    onFireTo: ActionLogic.turretFireTo(targetX, targetY);

    signal smokeTo (real targetX, real targetY)
    onSmokeTo: ActionLogic.turretSmokeTo(targetX, targetY);

    function performFiring() {
        changeStatus("READY");
        if (firing == true) {
            // Warning! This order is important for order markers!
            turret.firing = true;
            firing = false;
            actionFinished(unitIndex, __tempX, __tempY);
            queueOrderFinished();
        } else if (smoking == true) {
            // Warning! This order is important for order markers!
            turret.smoking = true;
            smoking = false;
            actionFinished(unitIndex, __tempX, __tempY);
            queueOrderFinished();
        }
    }

    id: root
    unitFileName: "Tank"
    unitType: "Generic tank"
    unitLogo: "../../../img/units/generic_unit_logo.png"
    rotationSpeed: 1
    turretRotationSpeed: 1
    maxSpeed: 1
    acceleration: 1
    unitWidth: 1
    unitHeight: 1

    RotationAnimation on turretRotation {
        id: turretRotationAnimation
        paused: running? root.paused : false
        duration: 3000
        direction: RotationAnimation.Shortest
        easing.type: Easing.InOutQuad

        onRunningChanged: {
            if (!running) {
                if (unitStatus != "STOPPED") {
                    if (reloadingTimer.running == false) {
                        performFiring();
                        reloadingTimer.start();
                    }
                }
            }
        }
    }

    Timer {
        id: reloadingTimer
        running: false
        interval: mainGunReloadingTime

        onRunningChanged: {
            if (running == true) {
                changeStatus("RELOADING");
            } else {
                performFiring();
            }
        }
    }

    VehicleExhaust {
        id: exhaust
        z: root.z + 4
        anchors.top: root.top
        anchors.left: root.left
        anchors.topMargin: (2 * centerY) - 3
        anchors.leftMargin: (2 * centerX) - 3
    }

    VehicleExplosion {
        id: explode
        z: root.z + 4
        anchors.fill: parent
    }

    // Implement transitions in children
    states: [
        State { name: "healthy_base" },
        State { name: "damaged_base" },
        State { name: "abandonned_base" },
        State { name: "destroyed_base"
            StateChangeScript {
                name: "destroyed_baseScript"
                script: { explode.pulse(1000) }
            }
        },
        State { name: "long destroyed_base" }
    ]
}
