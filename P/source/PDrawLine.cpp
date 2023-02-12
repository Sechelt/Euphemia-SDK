#include "LibInfo.h"
#include "PDrawLine.h"

#include "PCanvas.h"

#define PDrawLineBegin 0
#define PDrawLineMove 1
#define PDrawLineEnd 2

PDrawLine::PDrawLine( PCanvas *pCanvas )
    : PShapeBase( pCanvas )
{
}

PDrawLine::~PDrawLine()
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
QImage PDrawLine::getCopy()
{
    QRect r;
    r.setTopLeft( pointBegin );
    r.setBottomRight( pointEnd );

    QImage image( g_Context->getImage()->size(), QImage::Format_ARGB32 );
    image.fill( Qt::transparent );
    QPainter painter( &image );
    doPaint( &painter );

    return image.copy( r );
}

void PDrawLine::doDoubleClick( PMouseEvent *pEvent )
{ 
    Q_UNUSED( pEvent );
}

void PDrawLine::doPress( PMouseEvent *pEvent )
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


void PDrawLine::doMove( PMouseEvent *pEvent ) 
{
    // We only get here when a button is down but button is always none. Odd.
    // if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        {
            QPen pen = g_Context->getPen();
            pointEnd = pEvent->pos();
            update();
        }
        break;
    case StateManipulate:
        if ( pHandle ) doMoveHandle( pEvent->pos() );
        break;
    }
}

void PDrawLine::doRelease( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        if ( pCanvas->getAutoCommit() ) return doCommit();
        doManipulateState();
        break;
    case StateManipulate:
        break;
    }
}

void PDrawLine::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );
    QPainter painter( g_Context->getImage());
    doPaint( &painter );
    emit signalCommitted();
    doIdleState();
}

void PDrawLine::doPaint( QPainter *pPainter )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setBrush( g_Context->getBrush() );
    pPainter->setFont( g_Context->getFont() );
    // paint
    pPainter->drawLine( pointBegin, pointEnd );                         
}

void PDrawLine::doDrawState( const QPoint &point )
{
    Q_ASSERT( nState == StateIdle );
    pointBegin = pointEnd = point;
    nState = StateDraw;
    update();
    emit signalChangedState();
}

void PDrawLine::doManipulateState()
{
    Q_ASSERT( nState == StateDraw );
    doCreateHandles();
    nState = StateManipulate;
    emit signalChangedState();
}

void PDrawLine::doIdleState()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );

    if ( nState == StateDraw )
    {
        nState = StateIdle;
    }
    else if ( nState == StateManipulate )
    {
        doDeleteHandles();
        nState = StateIdle;
    }
    update();
    emit signalChangedState();
}

void PDrawLine::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    // their parent will be the viewport so...
    QPoint pointViewBegin   = pView->mapFromScene( pointBegin );
    QPoint pointViewEnd     = pView->mapFromScene( pointEnd );

    // Order matters when handles share a position. Last handle will be found first.
    PHandle *pHandle;

    // PDrawLineBegin
    pHandle = new PHandle( pView, PHandle::TypeMovePoint, pointViewBegin );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawLineMove
    QRect r;
    r.setTopLeft( pointViewBegin );
    r.setBottomRight( pointViewEnd );
    r = r.normalized();

    pHandle = new PHandle( pView, PHandle::TypeDrag, r.center() );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawLineEnd
    pHandle = new PHandle( pView, PHandle::TypeMovePoint, pointViewEnd );
    vectorHandles.append( pHandle );
    pHandle->show();
}

void PDrawLine::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );

    // adjust our geometry
    if ( pHandle == vectorHandles[PDrawLineBegin] )
    {
        // move the begin 
        pointBegin = pointPos;
    }
    else if ( pHandle == vectorHandles[PDrawLineMove] )
    {
        // get diff
        QRect r;
        r.setTopLeft( pointBegin );
        r.setBottomRight( pointEnd );
        r = r.normalized();
        QPoint pointDiff = pointPos - r.center();
        // update points
        pointBegin += pointDiff;
        pointEnd += pointDiff;
    }
    else if ( pHandle == vectorHandles[PDrawLineEnd] )
    {
        // move the end 
        pointEnd = pointPos;
    }

    // adjust handles
    QPoint  pointViewBegin  = pView->mapFromScene( pointBegin );
    QPoint  pointViewPos    = pView->mapFromScene( pointPos );
    QPoint  pointViewEnd    = pView->mapFromScene( pointEnd );

    if ( pHandle == vectorHandles[PDrawLineBegin] )
    {
        pHandle->setCenter( pointViewBegin );
        // get center
        QRect r;
        r.setTopLeft( pointViewBegin );
        r.setBottomRight( pointViewEnd );
        // adjust move handle
        vectorHandles[PDrawLineMove]->setCenter( r.center() );
    }
    else if ( pHandle == vectorHandles[PDrawLineMove] )
    {
        vectorHandles[PDrawLineBegin]->setCenter( pointViewBegin );
        vectorHandles[PDrawLineMove]->setCenter( pointViewPos );
        vectorHandles[PDrawLineEnd]->setCenter( pointViewEnd );
    }
    else if ( pHandle == vectorHandles[PDrawLineEnd] )
    {
        pHandle->setCenter( pointViewEnd );
        // get center
        QRect r;
        r.setTopLeft( pointViewBegin );
        r.setBottomRight( pointViewEnd );
        // adjust move handle
        vectorHandles[PDrawLineMove]->setCenter( r.center() );
    }
    update();
}


