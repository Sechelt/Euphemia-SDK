#ifndef H_PCanvas
#define H_PCanvas

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
        ToolSelectRectangle,            /*!< Default                */
        ToolSelectEllipse,              /*!< Default                */
        ToolSelectPolygon,              /*!< Default                */
        ToolDraw,                       /*!< Default                */
        ToolDrawLine,                   /*!< Default                */
        ToolDrawRectangle,              /*!< Default                */
        ToolDrawEllipse,                /*!< Default                */
        ToolDrawPolygon,                /*!< Default                */
        ToolDrawRectangleFilled,        /*!< Default                */
        ToolDrawEllipseFilled,          /*!< Default                */
        ToolDrawPolygonFilled,          /*!< Default                */
        ToolFillFlood,                  /*!< Default                */
        ToolFillGradient,               /*!< Default                */
        ToolSelection,                  /*!< PlugIn - UseCurrent    */
        ToolPen,                        /*!< PlugIn - UseCurrent    */
        ToolBrush,                      /*!< PlugIn - UseCurrent    */
        ToolShape,                      /*!< PlugIn - UseCurrent    */
        ToolFill                        /*!< PlugIn - UseCurrent    */
    };

    enum States
    {
        StateInactive,
        StateActive,
        StateManipulate
    };

    PCanvas( QWidget *parent, const QSize &size = QSize( 1024, 768 ) );
    PCanvas( QWidget *parent, const QImage &image );

    void setZoom( WZoomWidget::FitTypes nFit, int nZoom );
    void setTool( Tools n );
    void setAutoCommit( bool ); 

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

    void doDrawCommit();
    void doDrawCancel();

    bool isModified() { return bModified; }
    bool isDrawing();

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
    bool        bAutoCommit     = false;
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

    // active draw object
    // 0-1 will be active
//    PSelection *    pSelection  = nullptr;
//    PLine *         pLine       = nullptr;
//    PEllipse *      pEllipse    = nullptr;
//    PRectangle *    pRectangle  = nullptr;

    States  nState = StateInactive;
    QPoint  pointBegin;
    QPoint  pointEnd;
};

#endif

