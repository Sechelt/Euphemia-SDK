/*
 * Copyright 2022-2023, Peter Harvey <pharvey@codebydesign.com>
 *
 * This file is part of Euphemia-SDK.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef H_PCanvas
#define H_PCanvas

#include "PPasteRectangle.h"
#include "PMagnifierSelection.h"
#include "PSelectEllipse.h"
#include "PSelectPolygon.h"
#include "PSelectRectangle.h"
#include "PDrawFreeHand.h"
#include "PDrawErase.h"
#include "PDrawSpray.h"
#include "PDrawLine.h"
#include "PDrawPolygon.h"
#include "PDrawPolygonFilled.h"
#include "PDrawPolyline.h"
#include "PDrawRectangle.h"
#include "PDrawRectangleFilled.h"
#include "PDrawEllipse.h"
#include "PDrawEllipseFilled.h"
#include "PDrawText.h"

class PGraphicsView;
class PGraphicsScene;

/*!
 * \brief A canvas which facilitates viewing/editing of a QImage.
 *  
 * This canvas has most of the logic for painting within it. The scene just passes the events (mouse events) 
 * to this canvas. 
 *  
 * The; scene, background, canvas and image are all expected to be the same size. The image can change size 
 * (ie after being loaded from a file or by a request to crop). In this case the others are synced to 
 * maintain the same size for all. 
 *  
 * Precision (double vs int)
 *  
 * The scene uses 'double' precision while a QImage is 'int' precision. The painting functionality is 'int' 
 * precision - inheriting the QImage limitation. 
 *  
 * The incoming 'double' coordinates (from mouse events) are converted to 'int' precision for internal use.
 * The outgoing 'int' coordinates (provided by boundingRect()) are converted to 'double'.
 *  
 * Fortunately; QPainter accepts double or int precision. 
 *  
 * \sa PGraphicsScene 
 * \sa PBackground 
 *  
 * \author pharvey (2/10/23)
 */
class PCanvas : public QGraphicsObject
{
    Q_OBJECT

    friend class PCanvasView;

public:
    enum Tools
    {
        ToolMagnifierSelection,         /*!< shape: select rectangle, can move but size set elsewhere*/ 
        ToolSelectRectangle,            /*!< shape: select using a rectangle                         */ 
        ToolSelectEllipse,              /*!< shape: select using an ellipse                          */ 
        ToolSelectPolygon,              /*!< shape: select using a polygon                           */ 
        ToolDrawFreeHand,               /*!< free: scribble                                          */ 
        ToolDrawSpray,                  /*!< free: emulate a spray can                               */ 
        ToolDrawErase,                  /*!< free: erase to expose background                        */ 
        ToolDrawLine,                   /*!< shape: draw a straight line                             */ 
        ToolDrawRectangle,              /*!< shape: draw an empty rectangle                          */ 
        ToolDrawEllipse,                /*!< shape: draw an empty ellipse                            */ 
        ToolDrawPolygon,                /*!< shape: draw an empty polygon                            */ 
        ToolDrawPolyline,               /*!< shape: draw polyline                                    */ 
        ToolDrawRectangleFilled,        /*!< shape: draw a filled rectangle                          */ 
        ToolDrawEllipseFilled,          /*!< shape: draw a filled ellipse                            */ 
        ToolDrawPolygonFilled,          /*!< shape: draw a filled polygon                            */ 
        ToolDrawText,                   /*!< shape: draw a text                                      */ 
        ToolFillFlood,                  /*!< fill: flood fill with; solid color, pattern or texture  */ 
        ToolFillGradient                /*!< fill: fill with gradient                                */ 
    };

    PCanvas();

    virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 ) override;
    virtual QRectF boundingRect() const override;

    void setImage( const QImage & );
    void setTool( Tools n );
    void setAutoCommit( bool ); 
    void setBackground( const QColor & ); // a default color for a new image - not the same as PBackground

    PGraphicsView *         getView();
    PGraphicsScene *        getScene();
    PShapeBase *            getShape() { return pShapeBase; }
    QImage                  getImage() { return image; }
    QImage                  getCopy();
    bool                    getAutoCommit();
    QString                 getFileName() { return stringFileName; }
    QColor                  getBackground() { return colorBackground; }
    Tools                   getTool() { return nTool; }

    void doDoubleClickEvent( QGraphicsSceneMouseEvent *pEvent );
    void doPressEvent( QGraphicsSceneMouseEvent *pEvent );
    void doMoveEvent( QGraphicsSceneMouseEvent *pEvent );
    void doReleaseEvent( QGraphicsSceneMouseEvent *pEvent );
    void doZoomChanged();

    bool doOpen( const QString &s = QString() );
    bool doSave();
    bool doSaveAs();
    bool doClose();
    void doCut();
    void doCopy();
    void doPaste();
    void doPaste( const QImage & );
    void doUndo();
    void doRedo();
    void doSelectAll();
    void doSelectNone();
    void doCommit();
    void doCancel();
    void doCrop();
    void doScale( int nX, int nY, Qt::AspectRatioMode n = Qt::IgnoreAspectRatio );
    void doFlipX();
    void doFlipY();
    void doRotate( int nDegrees );

    bool isModified() { return bModified; }
    bool isDrawing();
    bool isBackgroundTransparent() { return bBackgroundTransparent; }

    bool hasSelection();
    bool hasPaste() { return bPaste; }

    bool canCut(); 
    bool canCopy(); 
    bool canPaste(); 
    bool canUndo(); 
    bool canRedo(); 
    bool canCrop() { return canCopy(); } 
    bool canCommit(); 
    bool canCancel(); 

signals:
    void signalPos( const QPoint & );
    void signalChangedState();
    void signalChangedFileName( const QString & );
    void signalMagnifierMoved();

public slots:
    void print();

protected slots:
    void slotCommitted();

protected:

private:
    void setModified( bool b = true );
    
    void doFillFlood( const QPoint &pointSeed );    
    void doFillFloodColor( const QPoint &pointSeed );    
    void doFillFloodTiled( const QPoint &pointSeed );
    void doFillGradient( const QPoint &pointSeed );    
    void doClear();

    Tools       nTool                       = ToolDrawLine;
    QString     stringFileName;
    bool        bModified                   = false;
    QColor      colorBackground             = QColor( Qt::transparent );        /*!< erase tool will use this */
    bool        bBackgroundTransparent      = true;                             /*!< erase tool will use this */
    bool        bPaste                      = false;

    QImage          image;
    QImage          imagePreCommit;
    int             nMaxUndo = 10;
    QStack<QImage>  stackUndo;
    QStack<QImage>  stackRedo;

    // active tool (or not)
    PFreeBase *     pFreeBase   = nullptr;
    PShapeBase *    pShapeBase  = nullptr;
};

#endif

