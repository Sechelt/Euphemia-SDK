#include "LibInfo.h"
#include "PGraphicsView.h"

PGraphicsView::PGraphicsView( PGraphicsScene *pScene, QWidget *pParent )
    : QGraphicsView( pScene, pParent )
{
    Q_ASSERT(pScene);                                                                                       
    // \todo only way to catch this?                                                                        
    // - when document size changes and not triggering other events because all is still in viewport        
    connect( pScene, SIGNAL(sceneRectChanged(const QRectF &)), SLOT(slotSceneRectChanged(const QRectF &)) );

#if QT_VERSION < 0x060000
    setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing );
#else
    setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform );
#endif
}

void PGraphicsView::setScale( qreal n )
{

    // 1
qInfo() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ <<"]" << getScaleH() << n;
 //   scale( n, n );

    // 2
    /*
    QMatrix matrix;      
    matrix.scale( n, n );
    setMatrix( matrix ); 
    */

    // 3
    QTransform transform;     
    transform.scale( n, n );  
    setTransform( transform );

    emit signalChangedH( getOffsetH(), getScaleH(), getLengthH() );
    emit signalChangedV( getOffsetV(), getScaleV(), getLengthV() );
}

PGraphicsScene *PGraphicsView::getScene()
{
    QGraphicsScene *p = scene();
    if ( !p ) return nullptr;
    if ( !p->inherits( "PGraphicsScene" ) ) return nullptr;
    return (PGraphicsScene*)p;
}

PCanvas *PGraphicsView::getCanvas()
{
    PGraphicsScene *p = getScene();
    if (!p ) return nullptr;
    return p->getCanvas();
}

/*!
 * \brief The X offset between viewport.topleft and scene.topleft. 
 *  
 * Created to notify the ruler. 
 *  
 * \author pharvey (2/21/20)
 * 
 * \return int 
 */
qreal PGraphicsView::getOffsetH()
{
    // default to case where scene is larger than viewport - we rely on scroll bar values
    qreal nOffsetH = -horizontalScrollBar()->value();

    // handle case where entire width is in viewport - we assume scene is centered in viewport
    qreal nSceneWidth = sceneRect().width() * getScaleH();
    if ( nSceneWidth < viewport()->width() ) nOffsetH = (viewport()->width() - nSceneWidth) / 2;

    return nOffsetH;
}

/*!
 * \brief Horizontal scale in play. 
 *  
 * m11 = horizontal 
 * m22 = vertical 
 * 
 * \author pharvey (2/25/20)
 * 
 * \return qreal 
 */
qreal PGraphicsView::getScaleH()
{
    return transform().m11();
}

qreal PGraphicsView::getLengthH()
{
    return scene()->width();
}

/*!
 * \brief The Y offset between viewport.topleft and scene.topleft. 
 *  
 * Created to notify the ruler. 
 *  
 * \author pharvey (2/21/20)
 * 
 * \return qreal 
 */
qreal PGraphicsView::getOffsetV()
{
    // default to case where scene is larger than viewport - we rely on scroll bar values
    qreal nOffsetV = -verticalScrollBar()->value();

    // handle case where entire height is in viewport - we assume scene is centered in viewport
    qreal nSceneHeight = sceneRect().height() * getScaleV();
    if ( nSceneHeight < viewport()->height() ) nOffsetV = (viewport()->height() - nSceneHeight) / 2;

    return nOffsetV;
}

/*!
 * \brief Vertical scale in play. 
 *  
 * m11 = horizontal 
 * m22 = vertical 
 * 
 * \author pharvey (2/25/20)
 * 
 * \return qreal 
 */
qreal PGraphicsView::getScaleV()
{
    return transform().m22();
}

qreal PGraphicsView::getLengthV()
{
    return scene()->height();
}

void PGraphicsView::slotSceneRectChanged( const QRectF &r )
{
    Q_UNUSED( r );

    emit signalChangedH( getOffsetH(), getScaleH(), getLengthH() );
    emit signalChangedV( getOffsetV(), getScaleV(), getLengthV() );
}

void PGraphicsView::resizeEvent( QResizeEvent *pEvent )
{
    QGraphicsView::resizeEvent( pEvent );

    emit signalChangedH( getOffsetH(), getScaleH(), getLengthH() );
    emit signalChangedV( getOffsetV(), getScaleV(), getLengthV() );
}

void PGraphicsView::scrollContentsBy( int dx, int dy )
{
    QGraphicsView::scrollContentsBy( dx, dy );

    emit signalChangedH( getOffsetH(), getScaleH(), getLengthH() );
    emit signalChangedV( getOffsetV(), getScaleV(), getLengthV() );
}



