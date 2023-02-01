#include "LibInfo.h"
#include "PDrawRectangle.h"

#include "PCanvas.h"

#define PDrawRectangleBegin 0
#define PDrawRectangleMove 1
#define PDrawRectangleEnd 2

PDrawRectangle::PDrawRectangle( PCanvas *pCanvas, const QPoint &pointBegin )
    : PShapeBase( pCanvas )
{
    // init begin
    this->pointBegin = pointBegin;
    pointEnd = pointBegin;
    // init handles
    QRect r;
    r.setTopLeft( pointBegin );
    r.setBottomRight( pointEnd );
    setGeometry( r.normalized() );
    setVisible( true );
    doCreateHandles();
}

bool PDrawRectangle::doPress( QMouseEvent *pEvent )
{
    // IF handle pressed on THEN start manipulation
    pHandle = getHandle( pEvent->pos() );
    if ( pHandle )
    {
        // hide handles during manipulation
        // doShowHandles( false );
        return true;
    }

    // pressed on the shape (but not a handle)
    if ( geometry().contains( pEvent->pos() ) ) return true;

    // nothing to do here so return false
    // app will probably tell this to doCommit() then delete this
    return false;
}

bool PDrawRectangle::doMove( QMouseEvent *pEvent ) 
{
    // IF no handle moving THEN no manipulation
    if ( !pHandle ) return true;

    doMoveHandle( pEvent->pos() );

    return true;
}

bool PDrawRectangle::doRelease( QMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );
    // IF no handle moving THEN no manipulation
    if ( !pHandle ) return true;

    // show handles in case we are going to do more manipulation
    doShowHandles();

    return true;
}

void PDrawRectangle::doCommit()
{
    QPainter painter( g_Context->getImage() );
    doPaint( &painter, pointBegin, pointEnd );

    doDeleteHandles();
}

void PDrawRectangle::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED( pEvent );
    QPainter painter( this );
    doPaint( &painter, mapFromParent( pointBegin ), mapFromParent( pointEnd ) );
}

void PDrawRectangle::doPaint( QPainter *pPainter, const QPoint &pointBegin, const QPoint &pointEnd )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );

    // paint
    QRect r( pointBegin, pointEnd );
    r = r.normalized();
    pPainter->drawRect( r );                         
}

void PDrawRectangle::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    PHandle *pHandle;
    QRect r( pointBegin, pointEnd );
    r = r.normalized();

    // Order matters when handles share a position. Last handle will be found first.

    // PDrawRectangleBegin
    pHandle = new PHandle( pCanvas, PHandle::TypeMovePoint, pointBegin );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawRectangleMove
    pHandle = new PHandle( pCanvas, PHandle::TypeMove, r.center() );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawRectangleEnd
    pHandle = new PHandle( pCanvas, PHandle::TypeMovePoint, pointEnd );
    vectorHandles.append( pHandle );
    pHandle->show();
}

void PDrawRectangle::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );

    if ( pHandle == vectorHandles[PDrawRectangleBegin] )
    {
        // move the begin 
        pointBegin = pointPos;
        pHandle->setCenter( pointBegin );
        // set canvas geometry
        QRect r( pointBegin, pointEnd );
        r = r.normalized();
        setGeometry( getGeometry( r, g_Context->getPen().width() ) );
        // adjust move handle
        vectorHandles[PDrawRectangleMove]->setCenter( r.center() );
    }
    else if ( pHandle == vectorHandles[PDrawRectangleMove] )
    {
        // get diff
        QRect r( pointBegin, pointEnd );
        r = r.normalized();
        QPoint pointDiff = pointPos - r.center();
        // update points
        pointBegin += pointDiff;
        pointEnd += pointDiff;
        // set canvas geometry
        r.setTopLeft( pointBegin );
        r.setBottomRight( pointEnd );
        r = r.normalized();
        setGeometry( getGeometry( r, g_Context->getPen().width() ) );
        // adjust all handles
        vectorHandles[PDrawRectangleBegin]->setCenter( pointBegin );
        vectorHandles[PDrawRectangleMove]->setCenter( pointPos );
        vectorHandles[PDrawRectangleEnd]->setCenter( pointEnd );
    }
    else if ( pHandle == vectorHandles[PDrawRectangleEnd] )
    {
        // move the end 
        pointEnd = pointPos;
        pHandle->setCenter( pointEnd );
        // set canvas geometry
        QRect r( pointBegin, pointEnd );
        r = r.normalized();
        setGeometry( getGeometry( r, g_Context->getPen().width() ) );
        // adjust move handle
        vectorHandles[PDrawRectangleMove]->setCenter( r.center() );
    }
}



