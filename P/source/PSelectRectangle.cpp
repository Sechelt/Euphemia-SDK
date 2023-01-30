#include "LibInfo.h"
#include "PSelectRectangle.h"

PSelectRectangle::PSelectRectangle( PCanvas *pCanvas, const QPoint &pointBegin )
    : PDrawRectangle( pCanvas, pointBegin )
{
}

void PSelectRectangle::doPaint( QPainter *pPainter, const QPoint &pointBegin, const QPoint &pointEnd )
{
    //
    pPainter->setPen( QPen( Qt::DashLine ) );

    // paint
    QRect r( pointBegin, pointEnd );
    r = r.normalized();
    pPainter->drawRect( r );                         
}


