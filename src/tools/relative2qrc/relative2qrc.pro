#-------------------------------------------------
#
# Project created by QtCreator 2012-01-08T12:58:40
# CloseCombatFree
# Author (c) Tomasz 'sierdzio' Siekierda
# sierdzio@gmail.com
# sierdzio.com
#-------------------------------------------------

include(../../globalImports.pri)

TARGET = convert2qrc

DESTDIR = $${EXEC_DIRECTORY}/tools
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}
RCC_DIR = $${BUILD_DIRECTORY}

TEMPLATE = app

HEADERS += \ 
    converterflags.h \
    converterfile.h \
    ../../ccferror.h \
    convertercore.h \
    converterqrcgenerator.h

SOURCES += \ 
    converterflags.cpp \
    converterfile.cpp \
    ../../ccferror.cpp \
    convertercore.cpp \
    convertermain.cpp \
    converterqrcgenerator.cpp

RESOURCES += \
    converterResource.qrc
