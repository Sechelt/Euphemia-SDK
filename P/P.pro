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
        include/PDrawEllipse.h \
        include/PDrawLine.h \
        include/PDrawRectangle.h \
        include/PFontToolBar.h \
        include/PHandle.h \
        include/PPenToolBar.h \
        include/PShapeBase.h

SOURCES += \
        source/LibInfo.h \
        source/PBrushToolBar.cpp \
        source/PCanvas.cpp \
        source/PCanvasView.cpp \
        source/PContext.cpp \
        source/PDrawEllipse.cpp \
        source/PDrawLine.cpp \
        source/PDrawRectangle.cpp \
        source/PFontToolBar.cpp \
        source/PHandle.cpp \
        source/PPenToolBar.cpp \
        source/PShapeBase.cpp


