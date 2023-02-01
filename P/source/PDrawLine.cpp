#include "LibInfo.h"
#include "PDrawLine.h"

#include "PCanvas.h"
#include "PPenToolBar.h"

#define PDrawLineBegin 0
#define PDrawLineMove 1
#define PDrawLineEnd 2

PDrawLine::PDrawLine( PCanvas *pCanvas, const QPoint &pointBegin )
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

bool PDrawLine::doPress( QMouseEvent *pEvent )
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

bool PDrawLine::doMove( QMouseEvent *pEvent ) 
{
    // IF no handle moving THEN no manipulation
    if ( !pHandle ) return true;

    doMoveHandle( pEvent->pos() );

    return true;
}

bool PDrawLine::doRelease( QMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );
    // IF no handle moving THEN no manipulation
    if ( !pHandle ) return true;

    // show handles in case we are going to do more manipulation
    doShowHandles();

    return true;
}

void PDrawLine::doCommit()
{
    QPainter painter( g_Context->getImage() );
    doPaint( &painter, pointBegin, pointEnd );

    doDeleteHandles();
}

void PDrawLine::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED( pEvent );
    QPainter painter( this );
    doPaint( &painter, mapFromParent( pointBegin ), mapFromParent( pointEnd ) );
}

void PDrawLine::doPaint( QPainter *pPainter, const QPoint &pointBegin, const QPoint &pointEnd )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setBrush( g_Context->getBrush() );
    pPainter->setFont( g_Context->getFont() );
    // paint
    pPainter->drawLine( pointBegin, pointEnd );                         
}

void PDrawLine::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    PHandle *pHandle;
    QRect r;

    r.setTopLeft( pointBegin );
    r.setBottomRight( pointEnd );
    r = r.normalized();

    // Order matters when handles share a position. Last handle will be found first.

    // PDrawLineBegin
    pHandle = new PHandle( pCanvas, PHandle::TypeMovePoint, pointBegin );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawLineMove
    pHandle = new PHandle( pCanvas, PHandle::TypeDrag, r.center() );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawLineEnd
    pHandle = new PHandle( pCanvas, PHandle::TypeMovePoint, pointEnd );
    vectorHandles.append( pHandle );
    pHandle->show();
}

void PDrawLine::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );

    if ( pHandle == vectorHandles[PDrawLineBegin] )
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
        vectorHandles[PDrawLineMove]->setCenter( r.center() );
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
        // set canvas geometry
        r.setTopLeft( pointBegin );
        r.setBottomRight( pointEnd );
        r = r.normalized();
        setGeometry( getGeometry( r, g_Context->getPen().width() ) );
        // adjust all handles
        vectorHandles[PDrawLineBegin]->setCenter( pointBegin );
        vectorHandles[PDrawLineMove]->setCenter( pointPos );
        vectorHandles[PDrawLineEnd]->setCenter( pointEnd );
    }
    else if ( pHandle == vectorHandles[PDrawLineEnd] )
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
        vectorHandles[PDrawLineMove]->setCenter( r.center() );
    }
}

//
// PLineToolBar
//
PLineToolBar::PLineToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pLayout->addWidget( new PPenToolBar( this ) );
    pLayout->addStretch( 10 );
}

