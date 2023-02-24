#include "LibInfo.h"
#include "PPanner.h"

PPanner::PPanner( PGraphicsScene *pScene, QWidget *pParent )
    : QGraphicsView( pScene, pParent )
{
    bPanning = false;
    setCursor( Qt::OpenHandCursor );

    // we want mouse events so we need interactive
    // setInteractive( false );

    // this is pointless as we always have the entire scene in the view
    // setDragMode( QGraphicsView::ScrollHandDrag );
}

PPanner::~PPanner()
{
}

QGraphicsView *PPanner::getView()
{
    QGraphicsView *pView = nullptr;
    QList<QGraphicsView*> listViews = scene()->views();

    if ( listViews.count() < 2 ) return nullptr;

    // find first one that is not us
    foreach( pView, listViews )
    {
        if ( pView != this ) return pView;
    }
    return nullptr;
}

void PPanner::slotUpdate() 
{ 
   viewport()->update();
}

void PPanner::resizeEvent( QResizeEvent *pEvent )
{
    Q_UNUSED(pEvent);
    // scale the entire scene to fit into our viewport
    fitInView( sceneRect(), Qt::KeepAspectRatio );
}

void PPanner::mousePressEvent( QMouseEvent *pEvent )
{
    if ( pEvent->button() != Qt::LeftButton ) return;
    bPanning = true;
    pointLast = pEvent->pos();
    setCursor( Qt::ClosedHandCursor );
}

/*!
 * \brief Scroll main view as we drag on panner.
 *  
 * We (the panner) is scaled so we get the Delta and unscale it. 
 * The main view may be scaled as well so we then scale the Delta to the main view.
 *  
 * \author pharvey (2/21/20)
 * 
 * \param pEvent 
 */
void PPanner::mouseMoveEvent( QMouseEvent *pEvent )
{
    if ( !bPanning ) return;
    QGraphicsView *pView = getView();
    if ( !pView ) return;

    // SCALE (of panner)
    qreal nXScale = transform().m11();
    qreal nYScale = transform().m22();

    // SCALE (of main view)
    qreal nXScaleMain = pView->transform().m11();
    qreal nYScaleMain = pView->transform().m22();
    
    // DELTA (in panner and scaled as per panner)
    qreal nXDeltaScaled = pEvent->pos().x() - pointLast.x();
    qreal nYDeltaScaled = pEvent->pos().y() - pointLast.y();

    // DELTA (in panner unscaled)
    qreal nXDeltaUnscaled = nXDeltaScaled / nXScale;
    qreal nYDeltaUnscaled = nYDeltaScaled / nYScale;

    // DELTA (scaled for main view)
    nXDeltaScaled = nXDeltaUnscaled * nXScaleMain;
    nYDeltaScaled = nYDeltaUnscaled * nYScaleMain;

    // APPLY DELTA (to main view)
    int nX = pView->horizontalScrollBar()->value();
    int nY = pView->verticalScrollBar()->value();

    int nXNew = nX + nXDeltaScaled;
    int nYNew = nY + nYDeltaScaled;

    pView->horizontalScrollBar()->setValue( nXNew );
    pView->verticalScrollBar()->setValue( nYNew );

    // \note this is protected and scroll bars are not 
    // pView->scrollContentsBy( nXDelta, nYDelta );

    pointLast = pEvent->pos();
    slotUpdate();
}

void PPanner::mouseReleaseEvent( QMouseEvent *pEvent )
{
    Q_UNUSED(pEvent);
    bPanning = false;
    setCursor( Qt::OpenHandCursor );
    scene()->update(); // scene will be confused without this update
}

/*!
 * \brief Draw a red box representing the main views viewport over the document.
 * 
 * \author pharvey (2/22/20)
 * 
 * \param pPainter 
 * \param rect 
 */
void PPanner::drawForeground( QPainter *pPainter, const QRectF &rect )
{
    Q_UNUSED(rect);
    // \to draw red rect to show area displayed in primary view
    // Not sure how this will work if there are split views for example.
    // Perhaps allow setView( int nIndex ) to tell us to use a certain view?
    // For now; just use the first view.
    QGraphicsView *pView = getView();
    if ( !pView ) return;

    QPen pen( Qt::red );
    pen.setWidth( 3 );
    pPainter->setPen( pen );

    // SCALE (of main view)
    qreal nXScaleMain = pView->transform().m11();
    qreal nYScaleMain = pView->transform().m22();

    // pPainter will automatically scale to panner but still need to apply scale 
    // from main view 
    QRectF r( pView->horizontalScrollBar()->value() / nXScaleMain,
              pView->verticalScrollBar()->value() / nYScaleMain,
              pView->viewport()->width() / nXScaleMain,
              pView->viewport()->height() / nYScaleMain );
    pPainter->drawRect( r );
}

/*!
 * \brief Override so we can prevent the background from being displayed for us. 
 *  
 * Drawing the pages and grid etc blurs the image to the point of not being able to 
 * see any objects. Here we drawBackground while indicating its for a panner - so 
 * minimal frills.
 * 
 * \author pharvey (11/19/19)
 * 
 * \param pPainter 
 * \param rect 
 */
/*
void PPanner::drawBackground( QPainter *pPainter, const QRectF &rect )
{                                                                     
    Q_UNUSED(rect);                                                   
                                                                      
    DGraphicsSceneProxy *p = (DGraphicsSceneProxy*)scene();           
    p->getDiagram()->drawBackground( pPainter, QRectF(), true );      
}                                                                     
*/



