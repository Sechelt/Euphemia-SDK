#ifndef H_PCanvas
#define H_PCanvas

#include <WZoomWidget.h>

#include "PPasteRectangle.h"
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

class PCanvasView;

class PCanvas : public QWidget
{
    Q_OBJECT

    friend class PCanvasView;

public:
    enum Tools
    {
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

    PCanvas( QWidget *parent, const QSize &size = QSize( 1024, 768 ) );
    PCanvas( QWidget *parent, const QImage &image );

    void setZoom( WZoomWidget::FitTypes nFit, int nZoom );
    void setTool( Tools n );
    void setAutoCommit( bool ); 
    void setBackground( const QColor & );

    int getZoom() { return nZoom; }
    WZoomWidget::FitTypes getFit() { return nFit; }
    QString getFileName() { return stringFileName; }
    QColor getBackground() { return colorBackground; }

    bool doOpen();
    bool doSave();
    bool doSaveAs();
    bool doClose();
    void doCut();
    void doCopy();
    void doPaste();
    void doUndo();
    void doRedo();

    void doCommit();
    void doCancel();

    bool isModified() { return bModified; }
    bool isDrawing();
    bool isBackgroundTransparent() { return bBackgroundTransparent; }

    bool canCut(); 
    bool canCopy(); 
    bool canPaste(); 
    bool canUndo(); 
    bool canRedo(); 
    bool canCommit(); 
    bool canCancel(); 

public slots:
    void print();

signals:
    void signalPos( const QPoint & );
    void signalChangedState();

protected slots:
    void slotCommitted();

protected:
    void mouseDoubleClickEvent(QMouseEvent *pEvent ) override;
    void mousePressEvent( QMouseEvent *pEvent ) override;
    void mouseMoveEvent( QMouseEvent *pEvent ) override;
    void mouseReleaseEvent( QMouseEvent *pEvent ) override;
    void paintEvent( QPaintEvent *pEvent ) override;
    void resizeEvent( QResizeEvent *pEvent ) override;

private:
    void setModified( bool b = true );
    
    void doFillFlood( const QPoint &pointSeed );    
    void doFillFloodColor( const QPoint &pointSeed );    
    void doFillFloodTiled( const QPoint &pointSeed );
    void doFillGradient( const QPoint &pointSeed );    
    void doClear();

    void resizeImage( QImage *image, const QSize &newSize );

    Tools       nTool           = ToolDrawLine;
    bool        bAutoCommit     = false;
    QString     stringFileName;
    bool        bModified       = false;
    QColor      colorBackground;
    bool        bBackgroundTransparent = true;

    // zoom
    WZoomWidget::FitTypes nFit = WZoomWidget::FitIgnore;
    int nZoom   = 100;

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

