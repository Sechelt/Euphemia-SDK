#include "LibInfo.h"
#include "PDrawEllipseFilled.h"

PDrawEllipseFilled::PDrawEllipseFilled( PCanvas *pCanvas, const QPoint &pointBegin )
    : PDrawEllipse( pCanvas, pointBegin )
{
}

void PDrawEllipseFilled::doPaint( QPainter *pPainter, const QPoint &pointBegin, const QPoint &pointEnd )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setBrush( g_Context->getBrush() );

    // paint
    QRect r( pointBegin, pointEnd );
    r = r.normalized();
    pPainter->drawEllipse( r );                         
}


