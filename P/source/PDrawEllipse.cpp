#include "LibInfo.h"
#include "PDrawEllipse.h"

#include "PCanvas.h"

PDrawEllipse::PDrawEllipse( PCanvas *pCanvas )
    : PDrawRectangle( pCanvas )
{
}

void PDrawEllipse::doPaint( QPainter *pPainter )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    // paint
    pPainter->drawEllipse( r.normalized() );                         
}


