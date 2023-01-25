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



