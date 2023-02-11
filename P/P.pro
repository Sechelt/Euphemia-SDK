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
        include/PBackground.h \
        include/PBrushToolBar.h \
        include/PCanvas.h \
        include/PContext.h \
        include/PDrawEllipse.h \
        include/PDrawEllipseFilled.h \
        include/PDrawErase.h \
        include/PDrawFreeHand.h \
        include/PDrawLine.h \
        include/PDrawPolygon.h \
        include/PDrawPolygonFilled.h \
        include/PDrawPolyline.h \
        include/PDrawRectangle.h \
        include/PDrawRectangleFilled.h \
        include/PDrawSpray.h \
        include/PDrawText.h \
        include/PFillFlood.h \
        include/PFontToolBar.h \
        include/PFreeBase.h \
        include/PGraphicsScene.h \
        include/PGraphicsView.h \
        include/PHandle.h \
#        include/PLayers.h \
        include/PPasteRectangle.h \
        include/PPenToolBar.h \
        include/PSelectEllipse.h \
        include/PSelectPolygon.h \
        include/PSelectRectangle.h \
        include/PShapeBase.h

SOURCES += \
        source/LibInfo.h \
        source/PBackground.cpp \
        source/PBrushToolBar.cpp \
        source/PCanvas.cpp \
        source/PContext.cpp \
        source/PDrawEllipse.cpp \
        source/PDrawEllipseFilled.cpp \
        source/PDrawErase.cpp \
        source/PDrawFreeHand.cpp \
        source/PDrawLine.cpp \
        source/PDrawPolygon.cpp \
        source/PDrawPolygonFilled.cpp \
        source/PDrawPolyline.cpp \
        source/PDrawRectangle.cpp \
        source/PDrawRectangleFilled.cpp \
        source/PDrawSpray.cpp \
        source/PDrawText.cpp \
        source/PFillFlood.cpp \
        source/PFontToolBar.cpp \
        source/PFreeBase.cpp \
        source/PGraphicsScene.cpp \
        source/PGraphicsView.cpp \
        source/PHandle.cpp \
#        source/PLayers.cpp \
        source/PPasteRectangle.cpp \
        source/PPenToolBar.cpp \
        source/PSelectEllipse.cpp \
        source/PSelectPolygon.cpp \
        source/PSelectRectangle.cpp \
        source/PShapeBase.cpp


