#include "LibInfo.h"
#include "PPasteRectangle.h"

PPasteRectangle::PPasteRectangle( PCanvas *pCanvas )
    : PDrawRectangle( pCanvas )
{
    image = QGuiApplication::clipboard()->image();
    doDraw( QPoint( 10, 10 ) );
    doManipulate();
}

PPasteRectangle::PPasteRectangle( PCanvas *pCanvas, const QImage &i )
    : PDrawRectangle( pCanvas )
{
    image = i;
    doDraw( QPoint( 10, 10 ) );
    doManipulate();
}

QRect PPasteRectangle::doPress( QMouseEvent *pEvent )
{
    QRect rectUpdate;

    if ( pEvent->button() != Qt::LeftButton ) return rectUpdate;

    switch ( nState )
    {
    case StateIdle:
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

QRect PPasteRectangle::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );

    QRect rectUpdate = r;
    QPainter painter( g_Context->getImage());
    doPaint( &painter );
    emit signalCommitted();
    doIdle();
    return rectUpdate;
}

bool PPasteRectangle::canCommit()
{
    return !image.isNull();
}

void PPasteRectangle::doPaint( QPainter *pPainter )
{
    pPainter->drawImage( r, image );
}

void PPasteRectangle::doDraw( const QPoint &point )
{
    Q_ASSERT( nState == StateIdle );
    r.setTopLeft( point );
    r.setSize( image.size() );
    nState = StateDraw;
    update();
    emit signalChangedState();
}

