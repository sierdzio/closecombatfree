#-------------------------------------------------
#
# Project created by QtCreator 2011-11-24T11:51:40
# CloseCombatFree
# Author (c) Tomasz 'sierdzio' Siekierda
# sierdzio@gmail.com
# sierdzio.com
#-------------------------------------------------

QT       = core gui declarative

TARGET = closecombatfree

OBJECTS_DIR = build
MOC_DIR = build

TEMPLATE = app

HEADERS += \
    base/ccfconfig.h \
    base/ccfconfigparser.h \
    base/ccferror.h \
    base/ccfconfigsaver.h \
    base/ccfmain.h

SOURCES += base/main.cpp \
    base/ccfconfig.cpp \
    base/ccfconfigparser.cpp \
    base/ccferror.cpp \
    base/ccfconfigsaver.cpp \
    base/ccfmain.cpp

OTHER_FILES += \
    base/main.qml \
    base/engineLogicHelpers.js \
    gui/UnitSelectionBox.qml \
    gui/RubberBand.qml \
    gui/RosterMenuEntry.qml \
    gui/RosterMenu.qml \
    gui/OrderMarker.qml \
    gui/DefenceSphere.qml \
    gui/ContextMenuEntry.qml \
    gui/ContextMenu.qml \
    maps/Map_tst1.qml \
    maps/Map.qml \
    scenarios/engineScenarioLogic.js \
    scenarios/Scenario_tst1.qml \
    scenarios/Scenario.qml \
    scenarios/Effect.qml \
    units/engineActionLogic.js \
    units/Unit.qml \
    units/tanks/TurretImage.qml \
    units/tanks/Turret.qml \
    units/tanks/Tank.qml \
    units/tanks/Hull.qml \
    units/tanks/tests/Tank_tst3_hull.qml \
    units/tanks/tests/Tank_tst3.qml \
    units/tanks/tests/Tank_tst2_turret.qml \
    units/tanks/tests/Tank_tst2.qml \
    units/tanks/tests/Tank_tst1_turret.qml \
    units/tanks/tests/Tank_tst1_hull.qml \
    units/tanks/tests/Tank_tst1.qml \
    gui/SimpleInfoBox.qml \
    gui/PopUpArrow.qml \
    gui/TopMenu.qml \
    gui/MenuEntry.qml \
    gui/ZoomBox.qml \
    gui/ZoomButton.qml \
    gui/BottomMenu.qml \
    units/Soldier.qml \
    gui/engineRosterHelper.js \
    gui/SoldierMenu.qml \
    units/unitSoldiers.js \
    gui/OptionsMenu.qml








































