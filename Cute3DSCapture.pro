TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    3dscapture.c

include(deployment.pri)
qtcAddDeployment()

LIBS += -lsfml-graphics -lsfml-window -lsfml-system

HEADERS += \
    3dscapture.h

LIBS     += -lusb-1.0

OTHER_FILES += \
    README.md

#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CFLAGS_RELEASE -= -O2
