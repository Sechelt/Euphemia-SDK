#include "LibInfo.h"
#include "PDrawRectangle.h"

#include "PCanvas.h"
#include "PPenToolBar.h"

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

QRect PDrawRectangle::doDoubleClick( QMouseEvent *pEvent )
{ 
    Q_UNUSED( pEvent );
    return QRect();
}

QRect PDrawRectangle::doPress( QMouseEvent *pEvent )
{
    QRect rectUpdate;

    if ( pEvent->button() != Qt::LeftButton ) return rectUpdate;

    switch ( nState )
    {
    case StateIdle:
        doDraw( pEvent->pos() );
        break;
    case StateDraw:
        break;
    case StateManipulate:
        pHandle = getHandle( pEvent->pos() );
        if ( !pHandle )
        {
            rectUpdate = doCommit();
        }
        break;
    }

    return rectUpdate;
}

QRect PDrawRectangle::doMove( QMouseEvent *pEvent ) 
{
    QRect rectUpdate;

    // We only get here when a button is down but button is always none. Odd.
    // if ( pEvent->button() != Qt::LeftButton ) return rectUpdate;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        r.setBottomRight( pEvent->pos() );
        rectUpdate = r;
        update();
        break;
    case StateManipulate:
        doMoveHandle( pEvent->pos() );
        break;
    }

    return rectUpdate;
}

QRect PDrawRectangle::doRelease( QMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    QRect rectUpdate;

    if ( pEvent->button() != Qt::LeftButton ) return rectUpdate;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        doManipulate();
        break;
    case StateManipulate:
        break;
    }

    return rectUpdate;
}

QRect PDrawRectangle::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );

    QRect rectUpdate = r;
    QPainter painter( g_Context->getImage());
    doPaint( &painter );
    emit signalCommitted();
    doIdle();
    return rectUpdate;
}

void PDrawRectangle::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED( pEvent );
    if ( nState != StateDraw && nState != StateManipulate ) return;
    QPainter painter( this );
    doPaint( &painter );
}

void PDrawRectangle::doPaint( QPainter *pPainter )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );

    // paint
    pPainter->drawRect( r.normalized() );                         
}

void PDrawRectangle::doDraw( const QPoint &point )
{
    Q_ASSERT( nState == StateIdle );
    r = QRect( point, QSize( 1, 1 ) );
    nState = StateDraw;
    update();
    emit signalChangedState();
}

void PDrawRectangle::doManipulate()
{
    Q_ASSERT( nState == StateDraw );
    doCreateHandles();
    nState = StateManipulate;
    emit signalChangedState();
}

void PDrawRectangle::doIdle()
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

    // Order matters when handles share a position. Last handle will be found first.
    PHandle *pHandle;

    // PDrawRectangleBegin
    pHandle = new PHandle( pCanvas, PHandle::TypeSizeTopLeft, r.topLeft() );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawRectangleMove
    pHandle = new PHandle( pCanvas, PHandle::TypeDrag, r.center() );
    vectorHandles.append( pHandle );
    pHandle->show();

    // PDrawRectangleEnd
    pHandle = new PHandle( pCanvas, PHandle::TypeSizeBottomRight, r.bottomRight() );
    vectorHandles.append( pHandle );
    pHandle->show();
}

void PDrawRectangle::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );

    if ( pHandle == vectorHandles[PDrawRectangleBegin] )
    {
        r.setTopLeft( pointPos );
        pHandle->setCenter( pointPos );
        // adjust move handle
        vectorHandles[PDrawRectangleMove]->setCenter( r.center() );
        doSyncHandleTypes();
    }
    else if ( pHandle == vectorHandles[PDrawRectangleMove] )
    {
        QPoint pointDiff = pointPos - r.center();
        r.setTopLeft( r.topLeft() + pointDiff );
        r.setBottomRight( r.bottomRight() + pointDiff );
        // adjust all handles
        vectorHandles[PDrawRectangleBegin]->setCenter( r.topLeft() );
        vectorHandles[PDrawRectangleMove]->setCenter( r.center() );
        vectorHandles[PDrawRectangleEnd]->setCenter( r.bottomRight() );
    }
    else if ( pHandle == vectorHandles[PDrawRectangleEnd] )
    {
        r.setBottomRight( pointPos );
        pHandle->setCenter( pointPos );
        // adjust move handle
        vectorHandles[PDrawRectangleMove]->setCenter( r.center() );
        doSyncHandleTypes();
    }
    update();
}

void PDrawRectangle::doSyncHandleTypes()
{
    QRect rect = r.normalized();

    if ( vectorHandles[PDrawRectangleBegin]->geometry().contains( rect.topLeft() ) ) vectorHandles[PDrawRectangleBegin]->setType( PHandle::TypeSizeTopLeft );             
    else if ( vectorHandles[PDrawRectangleBegin]->geometry().contains( rect.topRight() ) ) vectorHandles[PDrawRectangleBegin]->setType( PHandle::TypeSizeTopRight );      
    else if ( vectorHandles[PDrawRectangleBegin]->geometry().contains( rect.bottomLeft() ) ) vectorHandles[PDrawRectangleBegin]->setType( PHandle::TypeSizeBottomLeft );  
    else if ( vectorHandles[PDrawRectangleBegin]->geometry().contains( rect.bottomRight() ) ) vectorHandles[PDrawRectangleBegin]->setType( PHandle::TypeSizeBottomRight );
                                                                                                                                                                       
    if ( vectorHandles[PDrawRectangleEnd]->geometry().contains( rect.topLeft() ) ) vectorHandles[PDrawRectangleEnd]->setType( PHandle::TypeSizeTopLeft );                 
    else if ( vectorHandles[PDrawRectangleEnd]->geometry().contains( rect.topRight() ) ) vectorHandles[PDrawRectangleEnd]->setType( PHandle::TypeSizeTopRight );          
    else if ( vectorHandles[PDrawRectangleEnd]->geometry().contains( rect.bottomLeft() ) ) vectorHandles[PDrawRectangleEnd]->setType( PHandle::TypeSizeBottomLeft );      
    else if ( vectorHandles[PDrawRectangleEnd]->geometry().contains( rect.bottomRight() ) ) vectorHandles[PDrawRectangleEnd]->setType( PHandle::TypeSizeBottomRight );    
}

//
// PRectangleToolBar
//
PRectangleToolBar::PRectangleToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pLayout->addWidget( new PPenToolBar( this ) );
    pLayout->addStretch( 10 );
}

