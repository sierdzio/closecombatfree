#-------------------------------------------------
#
# Project created by QtCreator 2011-11-24T11:51:40
# CloseCombatFree
# Author (c) Tomasz 'sierdzio' Siekierda
# sierdzio@gmail.com
# sierdzio.com
#-------------------------------------------------

include(globalImports.pri)

QT = core gui qml quick

TARGET = closecombatfree

DESTDIR = $${EXEC_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}
RCC_DIR = $${BUILD_DIRECTORY}

TEMPLATE = app

HEADERS += \
    ccferror.h \
    ccfmain.h \
    ccfglobal.h \
    ccfgamemanager.h \
    \
    logic/ccfobjectbase.h \
    logic/ccfenginehelpers.h \
    logic/ccfscenariostate.h \
    \
    config/ccflogger.h \
    config/ccfconfig.h \
    config/ccfconfigdata.h \
    config/ccfconfigsaver.h \
    config/ccfconfigparser.h \
    config/ccfcommandlineparser.h \
    \
    qmlBase/ccfqmlbasescenario.h \
    qmlBase/ccfqmlbaseunit.h \
    qmlBase/ccfqmlbasesoldier.h \
    qmlBase/ccfqmlbaserostermenu.h \
    qmlBase/ccfqmlbasemap.h

SOURCES += main.cpp \
    ccferror.cpp \
    ccfmain.cpp \
    ccfglobal.cpp \
    ccfgamemanager.cpp \
    \
    logic/ccfenginehelpers.cpp \
    logic/ccfscenariostate.cpp \
    logic/ccfobjectbase.cpp \
    \
    config/ccflogger.cpp \
    config/ccfconfig.cpp \
    config/ccfconfigdata.cpp \
    config/ccfconfigsaver.cpp \
    config/ccfconfigparser.cpp \
    config/ccfcommandlineparser.cpp \
    \
    qmlBase/ccfqmlbasescenario.cpp \
    qmlBase/ccfqmlbaseunit.cpp \
    qmlBase/ccfqmlbasesoldier.cpp \
    qmlBase/ccfqmlbaserostermenu.cpp \
    qmlBase/ccfqmlbasemap.cpp

OTHER_FILES += \
    ../qml/main.qml \
    ../src/config/saveFileTemplate.txt \
    ../editor/ScenarioEditor.qml \
    ../qml/scenarios/Scenario.qml \
    \
    ../qml/maps/props/Prop.qml \
    ../qml/maps/props/PropRectangle.qml \
    ../qml/maps/props/PropCluster.qml \
    ../qml/maps/MapCluster.qml \
    \
    ../qml/effects/VehicleExhaust.qml \
    ../qml/effects/VehicleExplosion.qml \
    ../qml/effects/Shadow.qml \
    ../qml/effects/GunFireEffect.qml \
    ../qml/effects/Effect.qml \
    \
    ../qml/weather/Weather.qml \
    \
    ../qml/campaigns/CampaignMapList.qml \
    ../qml/campaigns/CampaignDescriptionGui.qml \
    ../qml/campaigns/CampaignList.qml \
    ../qml/gui/menus/PreferencesMenu2.qml

# Main menus:
OTHER_FILES += \
    ../qml/menus/MainMenu.qml \
    ../qml/menus/SavedGamesMenu.qml \
    ../qml/menus/CampaignMenu.qml \
    ../qml/menus/ScenarioMenu.qml \
    ../qml/menus/LoadGameMenu.qml

# GUI:
OTHER_FILES += \
    ../qml/gui/UnitSelectionBox.qml \
    ../qml/gui/RubberBand.qml \
    ../qml/gui/OrderMarker.qml \
    ../qml/gui/DefenceSphere.qml \
    ../qml/gui/menus/ContextMenu.qml \
    ../qml/gui/SimpleInfoBox.qml \
    ../qml/gui/PopUpArrow.qml \
    ../qml/gui/menus/TopMenu.qml \
    ../qml/gui/ZoomBox.qml \
    ../qml/gui/ZoomButton.qml \
    \
    ../qml/gui/menus/AboutMenu.qml \
    ../qml/gui/menus/RosterMenu.qml \
    ../qml/gui/menus/BottomMenu.qml \
    ../qml/gui/menus/SoldierMenu.qml \
    ../qml/gui/menus/OptionsMenu.qml \
    ../qml/gui/menus/StatusMessageMenu.qml \
    ../qml/gui/menus/PreferencesMenu.qml \
    \
    ../qml/gui/menuEntries/MenuEntry.qml \
    ../qml/gui/menuEntries/CardMenuEntry.qml \
    ../qml/gui/menuEntries/RosterMenuEntry.qml \
    ../qml/gui/menuEntries/LoadGameEntries.qml \
    ../qml/gui/menuEntries/ContextMenuEntry.qml \
    ../qml/gui/menuEntries/ScenarioMenuEntry.qml \
    ../qml/gui/menuEntries/StatusMessageMenuEntry.qml \
    ../qml/gui/menuEntries/PreferencesCheckboxEntry.qml \
    ../qml/gui/menuEntries/PreferencesScreenSizeEntry.qml \
    ../qml/gui/menuEntries/PreferencesSingleTextInputEntry.qml

# Units:
OTHER_FILES += \
    ../qml/units/Unit.qml \
    ../qml/units/tanks/TurretImage.qml \
    ../qml/units/tanks/Turret.qml \
    ../qml/units/tanks/Tank.qml \
    ../qml/units/tanks/Hull.qml

# Tst:
OTHER_FILES += \
    ../maps/Map_tst1.qml \
    ../maps/Map_tst2.qml \
    ../maps/Map_tst3.qml \
    ../maps/Map_tst4.qml \
    \
    ../campaigns/Campaign_tst1.qml \
    ../scenarios/Scenario_tst1.qml \
    ../scenarios/Scenario_tst2.qml \
    ../scenarios/Scenario_tst3.qml \
    ../scenarios/Scenario_tst4.qml \
    \
    ../units/tanks/tests/Tank_tst3_hull.qml \
    ../units/tanks/tests/Tank_tst3.qml \
    ../units/tanks/tests/Tank_tst2_turret.qml \
    ../units/tanks/tests/Tank_tst2.qml \
    ../units/tanks/tests/Tank_tst1_turret.qml \
    ../units/tanks/tests/Tank_tst1_hull.qml \
    ../units/tanks/tests/Tank_tst1.qml \
    \
    ../maps/props/HouseBasic_tst.qml \
    ../maps/props/Tree_tst.qml

#RESOURCES += \
#    skinResource.qrc \
#    tempResource.qrc \
#    imgResource.qrc \
#    coreResource.qrc
 
