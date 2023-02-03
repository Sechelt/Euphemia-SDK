#include "LibInfo.h"
#include "PSelectRectangle.h"

PSelectRectangle::PSelectRectangle( PCanvas *pCanvas )
    : PDrawRectangle( pCanvas )
{
}

QRect PSelectRectangle::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );
    // no commit for a select shape - we just go straight to idle
    doIdle();
    return QRect();
}

bool PSelectRectangle::canCommit()
{
    return false;
}

void PSelectRectangle::doPaint( QPainter *pPainter )
{
    //
    pPainter->setPen( QPen( Qt::DashLine ) );

    // paint
    pPainter->drawRect( r.normalized() );                         
}

