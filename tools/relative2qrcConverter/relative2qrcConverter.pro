#-------------------------------------------------
#
# Project created by QtCreator 2012-01-08T12:58:40
# CloseCombatFree
# Author (c) Tomasz 'sierdzio' Siekierda
# sierdzio@gmail.com
# sierdzio.com
#-------------------------------------------------

include(../../globalImports.pri)

TARGET = relative2qrcConverter

DESTDIR = $${EXEC_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}
RCC_DIR = $${BUILD_DIRECTORY}

TEMPLATE = app

HEADERS += \ 
    convertermain.h \
    converterflags.h \
    convertercore.h \
    converterfile.h \
    ../../src/ccferror.h

SOURCES += \ 
    convertermain.cpp \
    converterflags.cpp \
    convertercore.cpp \
    converterfile.cpp \
    ../../src/ccferror.cpp \
    converter.cpp
