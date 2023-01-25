#ifndef H_PCanvas
#define H_PCanvas

#include "P.h"

#include <W.h>
#include <WZoomWidget.h>

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
        ToolDrawLine,                   /*!< shape: draw a straight line                             */ 
        ToolDrawRectangle,              /*!< shape: draw an empty rectangle                          */ 
        ToolDrawEllipse,                /*!< shape: draw an empty ellipse                            */ 
        ToolDrawPolygon,                /*!< shape: draw an empty polygon                            */ 
        ToolDrawRectangleFilled,        /*!< shape: draw a filled rectangle                          */ 
        ToolDrawEllipseFilled,          /*!< shape: draw a filled ellipse                            */ 
        ToolDrawPolygonFilled,          /*!< shape: draw a filled polygon                            */ 
        ToolDrawText,                   /*!< shape: draw a text                                      */ 
        ToolFillFlood,                  /*!< fill: flood fill with; solid color, pattern or texture  */ 
        ToolFillGradient                /*!< fill: fill with gradient                                */ 
    };

    enum States
    {
        StateInactive,
        StateActive,
    };

    PCanvas( QWidget *parent, const QSize &size = QSize( 1024, 768 ) );
    PCanvas( QWidget *parent, const QImage &image );

    void setZoom( WZoomWidget::FitTypes nFit, int nZoom );
    void setTool( Tools n );

    int getZoom() { return nZoom; }
    WZoomWidget::FitTypes getFit() { return nFit; }
    QString getFileName() { return stringFileName; }

    bool doOpen();
    bool doSave();
    bool doSaveAs();
    bool doClose();
    void doCut();
    void doCopy();
    void doPaste();
    void doUndo();
    void doRedo();

    bool isModified() { return bModified; }

    bool canCut(); 
    bool canCopy(); 
    bool canPaste(); 
    bool canUndo(); 
    bool canRedo(); 

public slots:
    void print();

signals:
    void signalPos( const QPoint & );
    void signalStateChanged();

protected:
    void mousePressEvent( QMouseEvent *pEvent ) override;
    void mouseMoveEvent( QMouseEvent *pEvent ) override;
    void mouseReleaseEvent( QMouseEvent *pEvent ) override;
    void paintEvent( QPaintEvent *pEvent ) override;
    void resizeEvent( QResizeEvent *pEvent ) override;

    void doMousePressDraw( QMouseEvent *pEvent );
    void doMouseMoveDraw( QMouseEvent *pEvent );
    void doMouseReleaseDraw( QMouseEvent *pEvent );

    void doMousePressManipulate( QMouseEvent *pEvent );
    void doMouseMoveManipulate( QMouseEvent *pEvent );
    void doMouseReleaseManipulate( QMouseEvent *pEvent );

private:
    void setModified( bool );
        
    void doClear();

    void resizeImage( QImage *image, const QSize &newSize );

    Tools       nTool           = ToolSelection;
    QString     stringFileName;
    bool        bModified       = false;

    // zoom
    WZoomWidget::FitTypes nFit = WZoomWidget::FitIgnore;
    int nZoom   = 100;

    QImage          image;
    int             nMaxUndo = 10;
    QStack<QImage>  stackUndo;
    QStack<QImage>  stackRedo;
    QPoint          pointLast;

    States  nState = StateInactive;
    QPoint  pointBegin;
    QPoint  pointEnd;
};

#endif

