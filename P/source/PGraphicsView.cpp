#include "LibInfo.h"
#include "PGraphicsView.h"

PGraphicsView::PGraphicsView( PGraphicsScene *pScene, QWidget *pParent )
    : QGraphicsView( pScene, pParent )
{
    Q_ASSERT(pScene);                                                                                       
    // \todo only way to catch this?                                                                        
    // - when document size changes and not triggering other events because all is still in viewport        
    // connect( pScene, SIGNAL(sceneRectChanged(const QRectF &)), SLOT(slotSceneRectChanged(const QRectF &)) );

#if QT_VERSION < 0x060000
    setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing );
#else
    setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform );
#endif
}

PGraphicsScene *PGraphicsView::getScene()
{
    QGraphicsScene *p = scene();
    Q_ASSERT( p );
    Q_ASSERT( p->inherits( "PGraphicsScene" ) );
    return (PGraphicsScene*)p;
}

PCanvas *PGraphicsView::getCanvas()
{
    PGraphicsScene *p = getScene();
    Q_ASSERT( p );
    return p->getCanvas();
}

qreal PGraphicsView::getScale()
{
    return transform().m11();
}

void PGraphicsView::slotZoomChanged( WZoomWidget::FitTypes nFit, int n )
{
    nZoomFit    = nFit;
    nZoom       = n;

    // get scale
    qreal nScale;
    switch ( nFit )
    {
    case WZoomWidget::FitWidth:
    case WZoomWidget::FitHeight:
    case WZoomWidget::FitAll:
        // pView->fitInView( pView->getScene()->sceneRect() );
    case WZoomWidget::FitIgnore:
        nScale = qreal(nZoom) / 100;
        break;
    }

    // apply scale
    setScale( nScale );
}

void PGraphicsView::setScale( qreal n )
{
    if ( n == getScale() ) return;

    QTransform transform;
    transform.scale( n, n );  
    setTransform( transform );

    emit signalScaleChanged();
}


