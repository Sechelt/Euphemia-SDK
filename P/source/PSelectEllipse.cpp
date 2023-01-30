#include "LibInfo.h"
#include "PSelectEllipse.h"

PSelectEllipse::PSelectEllipse( PCanvas *pCanvas, const QPoint &pointBegin )
    : PDrawEllipse( pCanvas, pointBegin )
{
}

void PSelectEllipse::doPaint( QPainter *pPainter, const QPoint &pointBegin, const QPoint &pointEnd )
{
    //
    pPainter->setPen( QPen( Qt::DashLine ) );

    // paint
    QRect r( pointBegin, pointEnd );
    r = r.normalized();
    pPainter->drawEllipse( r );                         
}


