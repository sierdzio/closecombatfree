#-------------------------------------------------
#
# Project created by QtCreator 2011-11-24T11:51:40
# CloseCombatFree
# Author (c) Tomasz 'sierdzio' Siekierda
# sierdzio@gmail.com
# sierdzio.com
#-------------------------------------------------

include(globalImports.pri)

QT       += declarative

TARGET = closecombatfree

DESTDIR = $${EXEC_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}
RCC_DIR = $${BUILD_DIRECTORY}

TEMPLATE = app

HEADERS += \
    config/ccfconfig.h \
    config/ccfconfigparser.h \
    config/ccfconfigsaver.h \
    ccferror.h \
    ccfmain.h

SOURCES += main.cpp \
    config/ccfconfig.cpp \
    config/ccfconfigparser.cpp \
    config/ccfconfigsaver.cpp \
    ccferror.cpp \
    ccfmain.cpp

OTHER_FILES += \
    ../qml/main.qml \
    ../qml/engineLogicHelpers.js \
    ../qml/gui/UnitSelectionBox.qml \
    ../qml/gui/RubberBand.qml \
    ../qml/gui/menuEntries/RosterMenuEntry.qml \
    ../qml/gui/RosterMenu.qml \
    ../qml/gui/OrderMarker.qml \
    ../qml/gui/DefenceSphere.qml \
    ../qml/gui/menuEntries/ContextMenuEntry.qml \
    ../qml/gui/ContextMenu.qml \
    ../qml/maps/Map.qml \
    ../qml/scenarios/engineScenarioLogic.js \
    ../qml/scenarios/Scenario.qml \
    ../qml/scenarios/Effect.qml \
    ../qml/units/engineActionLogic.js \
    ../qml/units/Unit.qml \
    ../qml/units/tanks/TurretImage.qml \
    ../qml/units/tanks/Turret.qml \
    ../qml/units/tanks/Tank.qml \
    ../qml/units/tanks/Hull.qml \
    ../qml/gui/SimpleInfoBox.qml \
    ../qml/gui/PopUpArrow.qml \
    ../qml/gui/TopMenu.qml \
    ../qml/gui/menuEntries/MenuEntry.qml \
    ../qml/gui/ZoomBox.qml \
    ../qml/gui/ZoomButton.qml \
    ../qml/gui/BottomMenu.qml \
    ../qml/units/Soldier.qml \
    ../qml/gui/SoldierMenu.qml \
    ../qml/gui/OptionsMenu.qml \
    ../qml/gui/StatusMessageMenu.qml \
    ../qml/gui/menuEntries/StatusMessageMenuEntry.qml \
    ../qml/maps/props/Prop.qml \
    ../qml/maps/props/PropRectangle.qml \
    \
    ../scenarios/Scenario_tst1.qml \
    ../units/tanks/tests/Tank_tst3_hull.qml \
    ../units/tanks/tests/Tank_tst3.qml \
    ../units/tanks/tests/Tank_tst2_turret.qml \
    ../units/tanks/tests/Tank_tst2.qml \
    ../units/tanks/tests/Tank_tst1_turret.qml \
    ../units/tanks/tests/Tank_tst1_hull.qml \
    ../units/tanks/tests/Tank_tst1.qml \
    ../maps/Map_tst1.qml \
    ../maps/Map_tst2.qml \
    ../maps/props/HouseBasic_tst.qml \
    ../maps/props/Tree_tst.qml \
    ../scenarios/Scenario_tst2.qml \
    ../qml/menus/ScenarioMenu.qml \
    ../maps/Map_tst3.qml \
    ../scenarios/Scenario_tst3.qml \
    ../qml/units/Order.qml \
    ../qml/units/tempJs.js \
    ../qml/gui/ScenarioMenuEntry.qml \
    ../qml/gui/PreferencesMenu.qml \
    ../qml/gui/menuEntries/PreferencesCheckboxEntry.qml \
    ../qml/gui/menuEntries/PreferencesScreenSizeEntry.qml \
    ../qml/gui/menuEntries/PreferencesSingleTextInputEntry.qml \
    ../src/config/saveFileTemplate.txt \
    ../qml/gui/LoadGameMenu.qml \
    ../qml/gui/menuEntries/LoadGameEntries.qml \
    ../qml/maps/props/PropCluster.qml \
    ../scenarios/Scenario_tst4.qml \
    ../editor/ScenarioEditor.qml

RESOURCES += \
    coreResource.qrc
#    skinResource.qrc \
#    tempResource.qrc \
#    imgResource.qrc
 
