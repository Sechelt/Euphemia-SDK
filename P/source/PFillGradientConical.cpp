#include "LibInfo.h"
#include "PFillGradientConical.h"

#include "PContext.h"
#include "PCanvas.h"

#define PFillGradientConicalSeed 0
#define PFillGradientConicalStart 1
#define PFillGradientConicalStop 2

PFillGradientConical::PFillGradientConical( PCanvas *pCanvas )
    : PShapeBase( pCanvas )
{
}

PFillGradientConical::~PFillGradientConical()
{
    doCancel();
}

/*!
 * \brief Return a copy of the CURRENT SHAPE.
 *  
 * This is done by getting doPaint to draw on an empty, temp, canvas and then 
 * copying from that canvas. 
 *  
 * \author pharvey (2/6/23)
 * 
 * \return QImage 
 */
QImage PFillGradientConical::getCopy()
{
    QImage image( g_Context->getImage()->size(), QImage::Format_ARGB32 );
    image.fill( Qt::transparent );
    QPainter painter( &image );
    doPaint( &painter );

    return image.copy( polygon.boundingRect() );
}

/*!
 * \brief Switch from drawing mode to manipulating mode.
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pEvent 
 * 
 * \return QRect 
 */
void PFillGradientConical::doDoubleClick( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );
}

/*!
 * \brief Create a new point (drawing mode) or start to move a point (manipulating mode).
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pEvent 
 * 
 * \return bool 
 */
void PFillGradientConical::doPress( PMouseEvent *pEvent )
{
    if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        doDrawState( pEvent->pos() );
        break;
    case StateDraw:
        break;
    case StateManipulate:
        pHandle = getHandle( pEvent->pos() );
        if ( !pHandle ) doCommit();
        break;
    }
}

void PFillGradientConical::doMove( PMouseEvent *pEvent ) 
{
    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        break;
    case StateManipulate:
        if ( pHandle ) doMoveHandle( pEvent->pos() );
        break;
    }
}

void PFillGradientConical::doRelease( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        break;
    case StateManipulate:
        pHandle = nullptr;
        break;
    }
}

void PFillGradientConical::doCommit()
{
    Q_ASSERT( nState == StateManipulate );

    QLinearGradient gradient( pointStart, pointStop );
    gradient.setColorAt( 0, Qt::black );
    gradient.setColorAt( 1, Qt::white );

    QPainter painter( g_Context->getImage());
    QBrush brush( gradient );
    painter.setBrush( brush );

    QRect r( pointStart, pointStop );

    painter.drawRect( r );


    emit signalCommitted();
    doIdleState();
}

void PFillGradientConical::doPaint( QPainter *pPainter )
{
    if ( nState != StateManipulate ) return;

    // paint gradient 
    {
/*
        QLinearGradient gradient( pointStart, pointStop );                           
        gradient.setColorAt( 0, Qt::black );                                         
        gradient.setColorAt( 0.5, Qt::yellow );                                      
        gradient.setColorAt( 0.75, Qt::blue );                                       
        gradient.setColorAt( 1, Qt::white );                                         
        gradient.setSpread( QGradient::ReflectSpread ); // for linear and radial only
*/

        QGradient gradient( QGradient::MeanFruit );
        pPainter->setBrush( QBrush( gradient ) );
        pPainter->setPen( QPen( Qt::NoPen ) );
        pPainter->drawPolygon( polygon );
    }

    // paint line between start/stop points
    {
        QPen pen( Qt::darkGray );
        pen.setStyle( Qt::DashLine );
        pen.setWidth( 2 );
        pPainter->setPen( pen );
        pPainter->setBrush( Qt::NoBrush );
        pPainter->drawLine( pointStart, pointStop );
    }
}

void PFillGradientConical::doDrawState( const QPoint &point )
{
    Q_ASSERT( nState == StateIdle );

    pointStart = pointStop = pointSeed = point;
    polygon = getPolygon( pointSeed );
    nState = StateDraw;
    doManipulateState();
}

void PFillGradientConical::doManipulateState()
{
    Q_ASSERT( nState == StateDraw );
    doCreateHandles();
    nState = StateManipulate;
    emit signalChangedState();
}

void PFillGradientConical::doIdleState()
{
    Q_ASSERT( nState == StateManipulate );

    doDeleteHandles();
    polygon.clear();
    nState = StateIdle;
    update();
    emit signalChangedState();
}


/*!
 * \brief Create handles.
 *  
 * Order matters when handles share a position. Last handle will be found first. 
 *  
 */
void PFillGradientConical::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    // Order matters when handles share a position. Last handle will be found first.
    PHandle *pHandle;

    // seed handle is always vectorHandles[0]
    pHandle = new PHandle( pView, PHandle::TypeFillSeed, pView->mapFromScene( pointSeed ) );
    vectorHandles.append( pHandle );
    pHandle->show();

    // start handle is always vectorHandles[1]
    pHandle = new PHandle( pView, PHandle::TypeFillStart, pView->mapFromScene( pointStart ) );
    vectorHandles.append( pHandle );
    pHandle->show();

    // stop handle is always vectorHandles[2]
    pHandle = new PHandle( pView, PHandle::TypeFillStop, pView->mapFromScene( pointStop ) );
    vectorHandles.append( pHandle );
    pHandle->show();

}

void PFillGradientConical::doSyncHandles()
{
}

void PFillGradientConical::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );

    QPoint pointViewPos = pView->mapFromScene( pointPos );

    if ( pHandle == vectorHandles[PFillGradientConicalSeed] )
    {
    }
    else if ( pHandle == vectorHandles[PFillGradientConicalStart] )
    {
        pHandle->setCenter( pointViewPos );
        pointStart = pointPos;
        update();
    }
    else if ( pHandle == vectorHandles[PFillGradientConicalStop] )
    {
        pHandle->setCenter( pointViewPos );
        pointStop = pointPos;
        update();
    }
}

/*!
 * \brief Returns a polygon representing a boundary. 
 *  
 * The boundary is defined by not being the color at pointSeed starting at pointSeed. 
 *  
 * Uses the 'Square tracing algorithm'. 
 *  
 * \author pharvey (2/17/23)
 * 
 * \param point  
 * 
 * \return QPolygon The inside of the polygon is the area outlined.
 */
QPolygon PFillGradientConical::getPolygon( const QPoint &pointSeed )
{
    QImage *    pImage      = g_Context->getImage();
    int         nX          = pointSeed.x();
    int         nY          = pointSeed.y();
    QColor      colorSeed   = pImage->pixelColor( pointSeed );
    QPolygon    polygon;

    // go west until we hit a boundary (or go off image)
    do
    {
        nX--;
        if ( isBoundary( pImage, colorSeed, QPoint( nX, nY ) ) ) break; 
    } while ( 1 );

    // start point
    QPoint pointStart( nX, nY );
    polygon.append( pointStart );

    //
    QPoint pointStep = getLeft( QPoint( 0, 1 ) );               
    QPoint point = pointStart + pointStep;
    while ( point != pointStart )
    {
        if ( isBoundary( pImage, colorSeed, point ) )
        {
            polygon.append( point );
            pointStep = getLeft( pointStep );
            point = point + pointStep;
        }
        else
        {
            point = point - pointStep;
            pointStep = getRight( pointStep );
            point = point + pointStep;
        }
    }

    return polygon;
}


