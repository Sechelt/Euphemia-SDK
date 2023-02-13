#include "LibInfo.h"
#include "PDrawRectangle.h"

#include "PCanvas.h"

#define PDrawRectangleBegin 0
#define PDrawRectangleMove 1
#define PDrawRectangleEnd 2

PDrawRectangle::PDrawRectangle( PCanvas *pCanvas )
    : PShapeBase( pCanvas )
{
}

PDrawRectangle::~PDrawRectangle()
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
QImage PDrawRectangle::getCopy()
{
    QImage image( g_Context->getImage()->size(), QImage::Format_ARGB32 );
    image.fill( Qt::transparent );
    QPainter painter( &image );
    doPaint( &painter );

    return image.copy( r );
}

void PDrawRectangle::doDoubleClick( PMouseEvent *pEvent )
{ 
    Q_UNUSED( pEvent );
}

void PDrawRectangle::doPress( PMouseEvent *pEvent )
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

void PDrawRectangle::doMove( PMouseEvent *pEvent ) 
{
    // We only get here when a button is down but button is always none. Odd.
    // if ( pEvent->button() != Qt::LeftButton ) return rectUpdate;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        r.setBottomRight( pEvent->pos() );
        update();
        break;
    case StateManipulate:
        if ( pHandle ) doMoveHandle( pEvent->pos() );
        break;
    }
}

void PDrawRectangle::doRelease( PMouseEvent *pEvent )
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

void PDrawRectangle::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );

    QPainter painter( g_Context->getImage() );
    doPaint( &painter );
    emit signalCommitted();
    doIdleState();
}

void PDrawRectangle::doPaint( QPainter *pPainter )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );

    // paint
    pPainter->drawRect( r.normalized() );                         
}

void PDrawRectangle::doDrawState( const QPoint &point )
{
    Q_ASSERT( nState == StateIdle );
    r = QRect( point, QSize( 1, 1 ) );
    nState = StateDraw;
    update();
    emit signalChangedState();
}

void PDrawRectangle::doManipulateState()
{
    Q_ASSERT( nState == StateDraw );
    doCreateHandles();
    nState = StateManipulate;
    emit signalChangedState();
}

void PDrawRectangle::doIdleState()
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

void PDrawRectangle::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    // their parent will be the viewport so...
    QRect rectView = pView->mapFromScene( r ).boundingRect();

    // Order matters when handles share a position. Last handle will be found first.
    PHandle *pHandle;

    // PDrawRectangleBegin
    pHandle = new PHandle( pView, PHandle::TypeSizeTopLeft, rectView.topLeft() );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawRectangleMove
    pHandle = new PHandle( pView, PHandle::TypeDrag, rectView.center() );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawRectangleEnd
    pHandle = new PHandle( pView, PHandle::TypeSizeBottomRight, rectView.bottomRight() );
    vectorHandles.append( pHandle );
    pHandle->show();
}

void PDrawRectangle::doSyncHandles()
{
    QRect rectView = pView->mapFromScene( r.normalized() ).boundingRect();

    vectorHandles[PDrawRectangleBegin]->setCenter( rectView.topLeft() );
    vectorHandles[PDrawRectangleMove]->setCenter( rectView.center() );
    vectorHandles[PDrawRectangleEnd]->setCenter( rectView.bottomRight() );
}

void PDrawRectangle::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );

    // adjust our geometry
    if ( pHandle == vectorHandles[PDrawRectangleBegin] )
    {
        r.setTopLeft( pointPos );
    }
    else if ( pHandle == vectorHandles[PDrawRectangleMove] )
    {
        QPoint pointDiff = pointPos - r.center();
        r.setTopLeft( r.topLeft() + pointDiff );
        r.setBottomRight( r.bottomRight() + pointDiff );
    }
    else if ( pHandle == vectorHandles[PDrawRectangleEnd] )
    {
        r.setBottomRight( pointPos );
    }

    // adjust handles

    // their parent is the viewport so...
    QRect   rectView    = pView->mapFromScene( r ).boundingRect();
    QPoint  pointView   = pView->mapFromScene( pointPos );

    if ( pHandle == vectorHandles[PDrawRectangleBegin] )
    {
        pHandle->setCenter( pointView );
        vectorHandles[PDrawRectangleMove]->setCenter( rectView.center() );
        doSyncHandleTypes();
    }
    else if ( pHandle == vectorHandles[PDrawRectangleMove] )
    {
        vectorHandles[PDrawRectangleBegin]->setCenter( rectView.topLeft() );
        vectorHandles[PDrawRectangleMove]->setCenter( rectView.center() );
        vectorHandles[PDrawRectangleEnd]->setCenter( rectView.bottomRight() );
    }
    else if ( pHandle == vectorHandles[PDrawRectangleEnd] )
    {
        pHandle->setCenter( pointView );
        vectorHandles[PDrawRectangleMove]->setCenter( rectView.center() );
        doSyncHandleTypes();
    }
    update();
}

void PDrawRectangle::doSyncHandleTypes()
{
    QRect rectView = pView->mapFromScene( r.normalized() ).boundingRect();

    if ( vectorHandles[PDrawRectangleBegin]->geometry().contains( rectView.topLeft() ) ) vectorHandles[PDrawRectangleBegin]->setType( PHandle::TypeSizeTopLeft );             
    else if ( vectorHandles[PDrawRectangleBegin]->geometry().contains( rectView.topRight() ) ) vectorHandles[PDrawRectangleBegin]->setType( PHandle::TypeSizeTopRight );      
    else if ( vectorHandles[PDrawRectangleBegin]->geometry().contains( rectView.bottomLeft() ) ) vectorHandles[PDrawRectangleBegin]->setType( PHandle::TypeSizeBottomLeft );  
    else if ( vectorHandles[PDrawRectangleBegin]->geometry().contains( rectView.bottomRight() ) ) vectorHandles[PDrawRectangleBegin]->setType( PHandle::TypeSizeBottomRight );
                                                                                                                                                                       
    if ( vectorHandles[PDrawRectangleEnd]->geometry().contains( rectView.topLeft() ) ) vectorHandles[PDrawRectangleEnd]->setType( PHandle::TypeSizeTopLeft );                 
    else if ( vectorHandles[PDrawRectangleEnd]->geometry().contains( rectView.topRight() ) ) vectorHandles[PDrawRectangleEnd]->setType( PHandle::TypeSizeTopRight );          
    else if ( vectorHandles[PDrawRectangleEnd]->geometry().contains( rectView.bottomLeft() ) ) vectorHandles[PDrawRectangleEnd]->setType( PHandle::TypeSizeBottomLeft );      
    else if ( vectorHandles[PDrawRectangleEnd]->geometry().contains( rectView.bottomRight() ) ) vectorHandles[PDrawRectangleEnd]->setType( PHandle::TypeSizeBottomRight );    
}

