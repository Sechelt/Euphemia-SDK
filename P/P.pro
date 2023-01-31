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
        include/PDrawEllipseFilled.h \
        include/PDrawFreeHand.h \
        include/PDrawLine.h \
        include/PDrawPolygon.h \
        include/PDrawRectangle.h \
        include/PDrawRectangleFilled.h \
        include/PFontToolBar.h \
        include/PFreeBase.h \
        include/PHandle.h \
        include/PPenToolBar.h \
        include/PSelectEllipse.h \
        include/PSelectRectangle.h \
        include/PShapeBase.h

SOURCES += \
        source/LibInfo.h \
        source/PBrushToolBar.cpp \
        source/PCanvas.cpp \
        source/PCanvasView.cpp \
        source/PContext.cpp \
        source/PDrawEllipse.cpp \
        source/PDrawEllipseFilled.cpp \
        source/PDrawFreeHand.cpp \
        source/PDrawLine.cpp \
        source/PDrawPolygon.cpp \
        source/PDrawRectangle.cpp \
        source/PDrawRectangleFilled.cpp \
        source/PFontToolBar.cpp \
        source/PFreeBase.cpp \
        source/PHandle.cpp \
        source/PPenToolBar.cpp \
        source/PSelectEllipse.cpp \
        source/PSelectRectangle.cpp \
        source/PShapeBase.cpp


