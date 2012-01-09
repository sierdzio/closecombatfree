#-------------------------------------------------
#
# Project created by QtCreator 2012-01-08T12:58:40
# CloseCombatFree
# Author (c) Tomasz 'sierdzio' Siekierda
# sierdzio@gmail.com
# sierdzio.com
#-------------------------------------------------

include(../../src/globalImports.pri)

TARGET = relative2qrc

DESTDIR = $${EXEC_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}
RCC_DIR = $${BUILD_DIRECTORY}

TEMPLATE = app

HEADERS += \ 
    converterflags.h \
    converterfile.h \
    ../../src/ccferror.h \
    convertercore.h

SOURCES += \ 
    converterflags.cpp \
    converterfile.cpp \
    ../../src/ccferror.cpp \
    convertercore.cpp \
    convertermain.cpp

RESOURCES += \
    converterResource.qrc
