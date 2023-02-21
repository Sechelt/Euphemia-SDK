#include "LibInfo.h"
#include "PDrawEllipseFilled.h"

PDrawEllipseFilled::PDrawEllipseFilled( PCanvas *pCanvas )
    : PDrawEllipse( pCanvas )
{
}

void PDrawEllipseFilled::doPaint( QPainter *pPainter, bool )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setBrush( g_Context->getBrush() );

    // paint
    pPainter->drawEllipse( r.normalized() );                         
}


