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
        pointMouse = pEvent->pos();
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
    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        pointMouse = pEvent->pos();
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

    // if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        break;
    case StateManipulate:
        if ( pHandle )
        {
            if ( shouldRemovePoint() ) doRemovePoint();
            pHandle = nullptr;
        }
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
    
    if ( nState == StateDraw ) pPainter->drawLine( polygon.last(), pointMouse );
}

void PDrawPolyline::doDrawState( const QPoint &point )
{
    Q_ASSERT( nState == StateIdle );

    polygon.append( point );    // begin
    pointMouse = point;

    nState = StateDraw;
    update();
    emit signalChangedState();

    setAcceptHoverEvents( true ); // mouse tracking - mouse move event even when no button down
}

void PDrawPolyline::doManipulateState()
{
    Q_ASSERT( nState == StateDraw );
    doCreateHandles();
    nState = StateManipulate;
    emit signalChangedState();

    setAcceptHoverEvents( false );
}

void PDrawPolyline::doIdleState()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );

    if ( nState == StateDraw )
    {
        nState = StateIdle;
        setAcceptHoverEvents( false );
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
 * first (at bottom) followed by NewPoint handles and then MovePoint handles.
 *  
 * \author pharvey (2/1/23)
 */
void PDrawPolyline::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    // their parent will be the viewport so...
    QPolygon polygonView = pView->mapFromScene( polygon );

    // Order matters when handles share a position. Last handle will be found first.
    PHandle *pHandle;

    // move handle is always vectorHandles[0]
    pHandle = new PHandle( pView, PHandle::TypeDrag, polygonView.boundingRect().center() );
    vectorHandles.append( pHandle );
    pHandle->show();

    // add a handle between each point (moving it will create a new point)
    for ( int n = 1; n < polygonView.count(); n++ )
    {
        pHandle = new PHandle( pView, PHandle::TypeNewPoint, QRect( polygonView.at( n - 1 ), polygonView.at( n ) ).center() );
        vectorHandles.append( pHandle );
        pHandle->show();
    }

    // add a handle for each point
    for ( QPoint point : polygonView )
    {
        pHandle = new PHandle( pView, PHandle::TypeMovePoint, point );
        vectorHandles.append( pHandle );
        pHandle->show();
    }

}

void PDrawPolyline::doSyncHandles()
{
    QPolygon polygonView = pView->mapFromScene( polygon );
    int nHandle = 0;

    // move handle
    vectorHandles[nHandle]->setCenter( polygonView.boundingRect().center() );

    // new point handles
    for ( int n = 1; n < polygonView.count(); n++ )
    {
        vectorHandles[++nHandle]->setCenter( QRect( polygonView.at( n - 1 ), polygonView.at( n ) ).center() );
    }

    // point handles
    for ( int n = 0; n < polygonView.count(); n++ )
    {
        vectorHandles[++nHandle]->setCenter( polygonView[n] );
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

    // their parent will be the viewport so...
    QPolygon    polygonView     = pView->mapFromScene( polygon );

    int nFirstMovePointHandle = polygon.count(); // would be count() - 1 except we also have the DragHandle at index 0

    // move handle?
    if ( pHandle->getType() == PHandle::TypeDrag )
    {
        QPoint pointDelta = pointPos - pView->mapToScene( pHandle->getCenter() ).toPoint();
        // move points
        for ( int n = 0; n < polygon.count(); n++ )
        {
            polygon.replace( n, polygon.at( n ) + pointDelta );
        }
    }
    else if ( pHandle->getType() == PHandle::TypeMovePoint )
    {
        // just a single point handle
        polygon.replace( vectorHandles.indexOf( pHandle ) - nFirstMovePointHandle, pointPos );
    }
    else if ( pHandle->getType() == PHandle::TypeNewPoint )
    {
        QPoint point            = pView->mapToScene( pHandle->getCenter() ).toPoint();
        int nHandleNewPoint     = vectorHandles.indexOf( pHandle );
        int nPointNext          = nHandleNewPoint; // almost corresponds to polygon but less 1 to account for DragHandle
        int nHandlePointNext    = nFirstMovePointHandle + nPointNext;
        int nHandleNewPointNext = nHandleNewPoint + 1;

        // insert point
        polygon.insert( nPointNext, pView->mapToScene( pHandle->getCenter() ).toPoint() );

        // insert another new point handle
        pHandle = new PHandle( pView, PHandle::TypeNewPoint, point );
        vectorHandles.insert( nHandleNewPointNext, pHandle );
        pHandle->show();
        nHandlePointNext++; // we just shoved everything over by one

        // insert another point handle for the point we just inserted
        pHandle = new PHandle( pView, PHandle::TypeMovePoint, point );
        vectorHandles.insert( nHandlePointNext, pHandle );
        pHandle->show();
    }

    doSyncHandles();
    update();
}

// call shouldRemovePoint() before calling here
void PDrawPolyline::doRemovePoint()
{
    // remove point
    int nFirstMovePointHandle = polygon.count() + 1;
    int nHandle = vectorHandles.indexOf( pHandle );
    int nPoint = nHandle - nFirstMovePointHandle + 1;
    polygon.remove( nPoint );

    // remove point handle
    delete vectorHandles.takeAt( nHandle );

    // remove left or right NewPoint handle?
    if ( nHandle > nFirstMovePointHandle ) nHandle = nPoint;  // left
    else nHandle = nPoint + 1;  // right

    delete vectorHandles.takeAt( nHandle );

    doSyncHandles();
    update();
}

bool PDrawPolyline::shouldRemovePoint()
{
    // we must be working with an existing polygon point
    if ( !pHandle ) return false;
    if ( pHandle->getType() != PHandle::TypeMovePoint ) return false;
    // not enough points to consider removing one?
    if ( polygon.count() <= 2 ) return false;
    // on another point?
    PHandle *p = getHandleUnder( pHandle, PHandle::TypeMovePoint );
    if ( !p ) return false;
    // must be an adjacent point to eliminate a line segment - adjacent?
    if ( p != getHandlePrev( pHandle, PHandle::TypeMovePoint ) && p != getHandleNext( pHandle, PHandle::TypeMovePoint ) ) return false;

    return true;
}

void PDrawPolyline::doDump()
{
    qInfo() << polygon;
    for ( int n = 0; n < vectorHandles.count(); n++ )
    {
        PHandle *p = vectorHandles.at( n );
        qInfo() << n << ":" << p->getType() << p->getCenter();
    }
}

