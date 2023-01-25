# #########################################################
# COMMON
# #########################################################
TEMPLATE    = lib
TARGET      = P
DESTDIR     = ../lib

include(../Euphemia-SDK.pri)

CONFIG      += staticlib

INCLUDEPATH += ./include

#
# Currently needs Classic-SDK source installed along side Euphemia-SDK.
# 
INCLUDEPATH += ../../Classic-SDK/W/include

# #########################################################
# FILES
# #########################################################
RESOURCES   = P.qrc

HEADERS += \
        include/P.h \
        include/PBrushToolBar.h \
        include/PCanvas.h \
        include/PCanvasView.h \
        include/PContext.h \
        include/PFontToolBar.h \
        include/PPenToolBar.h

SOURCES += \
        source/LibInfo.h \
        source/PBrushToolBar.cpp \
        source/PCanvas.cpp \
        source/PCanvasView.cpp \
        source/PContext.cpp \
        source/PFontToolBar.cpp \
        source/PPenToolBar.cpp


