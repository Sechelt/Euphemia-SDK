#include "LibInfo.h"
#include "PSelectEllipse.h"

PSelectEllipse::PSelectEllipse( PCanvas *pCanvas )
    : PSelectRectangle( pCanvas )
{
}

void PSelectEllipse::doPaint( QPainter *pPainter )
{
    //
    pPainter->setPen( QPen( Qt::DashLine ) );

    // paint
    pPainter->drawEllipse( r.normalized() );                         
}


