#include "LibInfo.h"
#include "PDrawPolygon.h"

#include "PCanvas.h"

#define PDrawPolygonBegin 0
#define PDrawPolygonMove 1
#define PDrawPolygonEnd 2

PDrawPolygon::PDrawPolygon( PCanvas *pCanvas, const QPoint &pointBegin )
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

bool PDrawPolygon::doPress( QMouseEvent *pEvent )
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

bool PDrawPolygon::doMove( QMouseEvent *pEvent ) 
{
    // IF no handle moving THEN no manipulation
    if ( !pHandle ) return true;

    doMoveHandle( pEvent->pos() );

    return true;
}

bool PDrawPolygon::doRelease( QMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );
    // IF no handle moving THEN no manipulation
    if ( !pHandle ) return true;

    // show handles in case we are going to do more manipulation
    doShowHandles();

    return true;
}

void PDrawPolygon::doCommit()
{
    QPainter painter( g_Context->getImage() );
    doPaint( &painter, pointBegin, pointEnd );

    doDeleteHandles();
}

void PDrawPolygon::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED( pEvent );
    QPainter painter( this );
    doPaint( &painter, mapFromParent( pointBegin ), mapFromParent( pointEnd ) );
}

void PDrawPolygon::doPaint( QPainter *pPainter, const QPoint &pointBegin, const QPoint &pointEnd )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setBrush( g_Context->getBrush() );
    pPainter->setFont( g_Context->getFont() );
    // paint
    pPainter->drawLine( pointBegin, pointEnd );                         
}

void PDrawPolygon::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    PHandle *pHandle;
    QRect r;

    r.setTopLeft( pointBegin );
    r.setBottomRight( pointEnd );
    r = r.normalized();

    // Order matters when handles share a position. Last handle will be found first.

    // PDrawPolygonBegin
    pHandle = new PHandle( pCanvas, PHandle::TypeMovePoint, pointBegin );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawPolygonMove
    pHandle = new PHandle( pCanvas, PHandle::TypeMove, r.center() );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawPolygonEnd
    pHandle = new PHandle( pCanvas, PHandle::TypeMovePoint, pointEnd );
    vectorHandles.append( pHandle );
    pHandle->show();
}

void PDrawPolygon::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );

    if ( pHandle == vectorHandles[PDrawPolygonBegin] )
    {
        // move the begin 
        pointBegin = pointPos;
        pHandle->setCenter( pointBegin );
        // set canvas geometry
        QRect r;
        r.setTopLeft( pointBegin );
        r.setBottomRight( pointEnd );
        r = r.normalized();
        setGeometry( getGeometry( r, g_Context->getPen().width() ) );
        // adjust move handle
        vectorHandles[PDrawPolygonMove]->setCenter( r.center() );
    }
    else if ( pHandle == vectorHandles[PDrawPolygonMove] )
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
        // set canvas geometry
        r.setTopLeft( pointBegin );
        r.setBottomRight( pointEnd );
        r = r.normalized();
        setGeometry( getGeometry( r, g_Context->getPen().width() ) );
        // adjust all handles
        vectorHandles[PDrawPolygonBegin]->setCenter( pointBegin );
        vectorHandles[PDrawPolygonMove]->setCenter( pointPos );
        vectorHandles[PDrawPolygonEnd]->setCenter( pointEnd );
    }
    else if ( pHandle == vectorHandles[PDrawPolygonEnd] )
    {
        // move the end 
        pointEnd = pointPos;
        pHandle->setCenter( pointEnd );
        // set canvas geometry
        QRect r;
        r.setTopLeft( pointBegin );
        r.setBottomRight( pointEnd );
        r = r.normalized();
        setGeometry( getGeometry( r, g_Context->getPen().width() ) );
        // adjust move handle
        vectorHandles[PDrawPolygonMove]->setCenter( r.center() );
    }
}




