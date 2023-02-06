#include "LibInfo.h"
#include "PDrawLine.h"

#include "PCanvas.h"
#include "PPenToolBar.h"

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

QRect PDrawLine::doDoubleClick( QMouseEvent *pEvent )
{ 
    Q_UNUSED( pEvent );
    return QRect();
}

QRect PDrawLine::doPress( QMouseEvent *pEvent )
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


QRect PDrawLine::doMove( QMouseEvent *pEvent ) 
{
    QRect rectUpdate;

    // We only get here when a button is down but button is always none. Odd.
    // if ( pEvent->button() != Qt::LeftButton ) return rectUpdate;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        {
            QPen pen = g_Context->getPen();
            pointEnd = pEvent->pos();
            rectUpdate.setTopLeft( pointBegin - QPoint( pen.width(), pen.width() ) );
            rectUpdate.setBottomRight( pointEnd + QPoint( pen.width(), pen.width() ) );
            update();
        }
        break;
    case StateManipulate:
        doMoveHandle( pEvent->pos() );
        break;
    }

    return rectUpdate;
}

QRect PDrawLine::doRelease( QMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    QRect rectUpdate;

    if ( pEvent->button() != Qt::LeftButton ) return rectUpdate;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        if ( pCanvas->getAutoCommit() ) return doCommit();
        doManipulate();
        break;
    case StateManipulate:
        break;
    }

    return rectUpdate;
}

QRect PDrawLine::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );
    QRect rectUpdate;
    rectUpdate.setTopLeft( pointBegin );
    rectUpdate.setBottomRight( pointEnd );
    QPainter painter( g_Context->getImage());
    doPaint( &painter );
    emit signalCommitted();
    doIdle();
    return rectUpdate;
}

void PDrawLine::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED( pEvent );
    if ( nState != StateDraw && nState != StateManipulate ) return;
    QPainter painter( this );
    doPaint( &painter );
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

void PDrawLine::doDraw( const QPoint &point )
{
    Q_ASSERT( nState == StateIdle );
    pointBegin = pointEnd = point;
    nState = StateDraw;
    update();
    emit signalChangedState();
}

void PDrawLine::doManipulate()
{
    Q_ASSERT( nState == StateDraw );
    doCreateHandles();
    nState = StateManipulate;
    emit signalChangedState();
}

void PDrawLine::doIdle()
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
        // adjust move handle
        vectorHandles[PDrawLineMove]->setCenter( r.center() );
    }
    update();
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

