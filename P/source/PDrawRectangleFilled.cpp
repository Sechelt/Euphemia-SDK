#include "LibInfo.h"
#include "PDrawRectangleFilled.h"

PDrawRectangleFilled::PDrawRectangleFilled( PCanvas *pCanvas )
    : PDrawRectangle( pCanvas )
{
}

void PDrawRectangleFilled::doPaint( QPainter *pPainter, bool )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setBrush( g_Context->getBrush() );

    // paint
    pPainter->drawRect( r.normalized() );                         
}


