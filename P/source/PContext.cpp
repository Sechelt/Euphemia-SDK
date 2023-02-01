#include "LibInfo.h"
#include "PContext.h"

PContext* PContext::instance()
{
    static PContext instance;
    return &instance;
}

void PContext::setImage( QImage *p )
{
    if ( pImage == p ) return;
    pImage = p;
}

void PContext::setPen( const QPen &t )
{
    if ( pen == t ) return;
    pen = t;
    emit signalModified( pen );
}

void PContext::setBrush( const QBrush &t )
{
    if ( brush == t ) return;
    brush = t;
    emit signalModified( brush );
}

void PContext::setFont( const QFont &t )
{
    if ( font == t ) return;
    font = t;
    emit signalModified( font );
}

void PContext::setFreeHand( const PContextFreeHand &t )
{
    if ( freehand == t ) return;
    freehand = t;
    emit signalModified( freehand );
}

void PContext::setSpray( const PContextSpray &t )
{
    if ( spray == t ) return;
    spray = t;
    emit signalModified( spray );
}

void PContext::setText( const PContextText &t )
{
    if ( text == t ) return;
    text = t;
    emit signalModified( text );
}

void PContext::slotImage( QImage *p )
{
    setImage( p );
}

void PContext::slotPen( const QPen &t )
{
    setPen( t );
}

void PContext::slotBrush( const QBrush &t )
{
    setBrush( t );
}

void PContext::slotFont( const QFont &t )
{
    setFont( t );
}

void PContext::slotFreeHand( const PContextFreeHand &t )
{
    setFreeHand( t );
}

void PContext::slotSpray( const PContextSpray &t )
{
    setSpray( t );
}

void PContext::slotText( const PContextText &t )
{
    setText( t );
}



