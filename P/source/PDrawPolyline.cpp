#include "LibInfo.h"
#include "PDrawPolyline.h"

#include "PCanvas.h"

PDrawPolyline::PDrawPolyline( PCanvas *pCanvas )
    : PShapeBase( pCanvas )
{
}

PDrawPolyline::~PDrawPolyline()
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
QImage PDrawPolyline::getCopy()
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
void PDrawPolyline::doDoubleClick( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    if ( nState != StateDraw ) return;

    doManipulateState();

    if ( pCanvas->getAutoCommit() ) doCommit();
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
void PDrawPolyline::doPress( PMouseEvent *pEvent )
{
    if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        doDrawState( pEvent->pos() );
        polygon.boundingRect();
        break;
    case StateDraw:
        polygon.append( pEvent->pos() );
        polygon.boundingRect();
        update();
        break;
    case StateManipulate:
        pHandle = getHandle( pEvent->pos() );
        if ( !pHandle ) doCommit();
        break;
    }
}

/*!
 * \brief Move the last point (drawing mode) or the handle/point (manipulating mode).
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pEvent 
 * 
 * \return bool 
 */
void PDrawPolyline::doMove( PMouseEvent *pEvent ) 
{
    // We only get here when a button is down but button is always none. Odd.
    // if ( pEvent->button() != Qt::LeftButton ) return rectUpdate;
    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        polygon.replace( polygon.count() - 1, pEvent->pos() );
        update();
        break;
    case StateManipulate:
        if ( pHandle ) doMoveHandle( pEvent->pos() );
        break;
    }
}

void PDrawPolyline::doRelease( PMouseEvent *pEvent )
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
        break;
    }
}

/*!
 * \brief Commit the polygon to canvas. 
 *  
 * 
 * \author pharvey (2/1/23)
 */
void PDrawPolyline::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );

    QPainter painter( g_Context->getImage());
    doPaint( &painter );
    emit signalCommitted();
    doIdleState();
}

void PDrawPolyline::doPaint( QPainter *pPainter )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );

    // paint
    pPainter->drawPolyline( polygon );                         
}

void PDrawPolyline::doDrawState( const QPoint &point )
{
    Q_ASSERT( nState == StateIdle );

    polygon.append( point );    // begin
    polygon.append( point );    // current

    nState = StateDraw;
    update();
    emit signalChangedState();
}

void PDrawPolyline::doManipulateState()
{
    Q_ASSERT( nState == StateDraw );
    doCreateHandles();
    nState = StateManipulate;
    emit signalChangedState();
}

void PDrawPolyline::doIdleState()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );

    if ( nState == StateDraw )
    {
        nState = StateIdle;
    }
    else if ( nState == StateManipulate )
    {
        doDeleteHandles();
        polygon.clear();
        nState = StateIdle;
    }
    update();
    emit signalChangedState();
}


/*!
 * \brief Create handles.
 *  
 * Order matters when handles share a position. Last handle will be found first. 
 *  
 * It would be easiest to append the move handle at the end as this would mean there would 
 * be a direct correspondence between point indexs and handle indexs - but this would make 
 * the move handle the default when all handles happened to be stacked upon each other. 
 * This would make resizing a very small polygon difficult - so we have the move handle 
 * first (at bottom). 
 *  
 * \author pharvey (2/1/23)
 */
void PDrawPolyline::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    PHandle *pHandle;

    // move handle is always vectorHandles[0]
    pHandle = new PHandle( PHandle::TypeDrag, polygon.boundingRect().center() );
    vectorHandles.append( pHandle );
    pCanvas->scene()->addItem( pHandle );
    pHandle->show();

    // add a handle for each point
    for ( QPoint point : polygon )
    {
        pHandle = new PHandle( PHandle::TypeMovePoint, point );
        vectorHandles.append( pHandle );
        pCanvas->scene()->addItem( pHandle );
        pHandle->show();
    }
}

/*!
 * \brief Move current handle.
 *  
 * Only used during manipulation mode. 
 *  
 * The polygon will be moved if move handle otherwise we will move handle/point and 
 * adjust move handle so its center. 
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pointPos 
 */
void PDrawPolyline::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );

    // move handle?
    if ( pHandle == vectorHandles[0] )
    {
        QPoint pointDelta = pointPos - pHandle->getCenter().toPoint();
        // move points
        for ( int n = 0; n < polygon.count(); n++ )
        {
            polygon.replace( n, polygon.at( n ) + pointDelta );
            vectorHandles.at( n + 1 )->doMoveBy( pointDelta );
        }
        // move self
        vectorHandles.at( 0 )->doMoveBy( pointDelta );
        update();
        return;
    }

    // point handle
    pHandle->setCenter( pointPos );
    polygon.replace( vectorHandles.indexOf( pHandle ) - 1, pointPos );
    vectorHandles.at( 0 )->setCenter( polygon.boundingRect().center() );
    update();
}

