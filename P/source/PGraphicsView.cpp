#include "LibInfo.h"
#include "PGraphicsView.h"

#include "PGraphicsScene.h"
#include "PCanvas.h"

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
    setAlignment( Qt::AlignLeft | Qt::AlignTop );
}

PGraphicsScene *PGraphicsView::getScene()
{
    QGraphicsScene *p = scene();
    if ( !p ) 
    {
        qWarning("View has no scene.");
        return nullptr;
    }
    Q_ASSERT( p->inherits( "PGraphicsScene" ) );
    return (PGraphicsScene*)p;
}

PCanvas *PGraphicsView::getCanvas()
{
    PGraphicsScene *p = getScene();
    if ( !p ) 
    {
        qWarning("View has no scene.");
        return nullptr;
    }
    return p->getCanvas();
}

qreal PGraphicsView::getScale()
{
    return transform().m11();
}

void PGraphicsView::slotZoomChanged( WZoomWidget::FitTypes nFit, int n )
{
    if ( nFit == nZoomFit && n == nZoom ) return;

    nZoomFit    = nFit;
    nZoom       = n;

    // get scale
    switch ( nFit )
    {
    case WZoomWidget::FitWidth:
        break;
    case WZoomWidget::FitHeight:
        break;
    case WZoomWidget::FitAll:
        break;
    case WZoomWidget::FitIgnore:
        break;
    }

    qreal nScale = qreal(nZoom) / 100;

    // apply scale
    setScale( nScale );
    emit signalZoomChanged( nFit, nZoom );
}

void PGraphicsView::setScale( qreal n )
{
    if ( n == getScale() ) return;

    QTransform transform;
    transform.scale( n, n );  
    setTransform( transform );

    getScene()->getCanvas()->doZoomChanged();
}


