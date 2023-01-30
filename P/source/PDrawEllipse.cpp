#include "LibInfo.h"
#include "PDrawEllipse.h"

#include "PCanvas.h"

#define PDrawEllipseBegin 0
#define PDrawEllipseMove 1
#define PDrawEllipseEnd 2

PDrawEllipse::PDrawEllipse( PCanvas *pCanvas, const QPoint &pointBegin )
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

bool PDrawEllipse::doPress( QMouseEvent *pEvent )
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

bool PDrawEllipse::doMove( QMouseEvent *pEvent ) 
{
    // IF no handle moving THEN no manipulation
    if ( !pHandle ) return true;

    doMoveHandle( pEvent->pos() );

    return true;
}

bool PDrawEllipse::doRelease( QMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );
    // IF no handle moving THEN no manipulation
    if ( !pHandle ) return true;

    // show handles in case we are going to do more manipulation
    doShowHandles();

    return true;
}

void PDrawEllipse::doCommit()
{
    QPainter painter( g_Context->getImage() );
    doPaint( &painter, pointBegin, pointEnd );

    doDeleteHandles();
}

void PDrawEllipse::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED( pEvent );
    QPainter painter( this );
    doPaint( &painter, mapFromParent( pointBegin ), mapFromParent( pointEnd ) );
}

void PDrawEllipse::doPaint( QPainter *pPainter, const QPoint &pointBegin, const QPoint &pointEnd )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setFont( g_Context->getFont() );
    // paint
    QRect r( pointBegin, pointEnd );
    r = r.normalized();
    pPainter->drawEllipse( r );                         
}

void PDrawEllipse::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    PHandle *pHandle;
    QRect r( pointBegin, pointEnd );
    r = r.normalized();

    // Order matters when handles share a position. Last handle will be found first.

    // PDrawEllipseBegin
    pHandle = new PHandle( pCanvas, PHandle::TypeMovePoint, pointBegin );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawEllipseMove
    pHandle = new PHandle( pCanvas, PHandle::TypeMove, r.center() );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawEllipseEnd
    pHandle = new PHandle( pCanvas, PHandle::TypeMovePoint, pointEnd );
    vectorHandles.append( pHandle );
    pHandle->show();
}

void PDrawEllipse::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );

    if ( pHandle == vectorHandles[PDrawEllipseBegin] )
    {
        // move the begin 
        pointBegin = pointPos;
        pHandle->setCenter( pointBegin );
        // set canvas geometry
        QRect r( pointBegin, pointEnd );
        r = r.normalized();
        setGeometry( getGeometry( r, g_Context->getPen().width() ) );
        // adjust move handle
        vectorHandles[PDrawEllipseMove]->setCenter( r.center() );
    }
    else if ( pHandle == vectorHandles[PDrawEllipseMove] )
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
        vectorHandles[PDrawEllipseBegin]->setCenter( pointBegin );
        vectorHandles[PDrawEllipseMove]->setCenter( pointPos );
        vectorHandles[PDrawEllipseEnd]->setCenter( pointEnd );
    }
    else if ( pHandle == vectorHandles[PDrawEllipseEnd] )
    {
        // move the end 
        pointEnd = pointPos;
        pHandle->setCenter( pointEnd );
        // set canvas geometry
        QRect r( pointBegin, pointEnd );
        r = r.normalized();
        setGeometry( getGeometry( r, g_Context->getPen().width() ) );
        // adjust move handle
        vectorHandles[PDrawEllipseMove]->setCenter( r.center() );
    }
}



