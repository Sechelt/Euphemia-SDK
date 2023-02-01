#include "LibInfo.h"
#include "PDrawFreeHand.h"

#include "PCanvas.h"
#include "PPenToolBar.h"

PDrawFreeHand::PDrawFreeHand( PCanvas *pCanvas )
    : PFreeBase( pCanvas )
{
}

QRect PDrawFreeHand::doPress( QMouseEvent *pEvent )
{
    pointLast = pEvent->pos();
    return QRect();
}

QRect PDrawFreeHand::doMove( QMouseEvent *pEvent )
{
    return doDrawLine( pEvent->pos() );
}

QRect PDrawFreeHand::doRelease( QMouseEvent *pEvent )
{
    return doDrawLine( pEvent->pos() );
}

QRect PDrawFreeHand::doDrawLine( const QPoint &point )
{
    // draw line
    QPainter painter( g_Context->getImage() );
    painter.setPen( g_Context->getPen() );
    painter.drawLine( pointLast, point );
    // calc rect to be updated - factor in pen width
    int nW = g_Context->getPen().width();
    QRect r( pointLast, point );
    r = r.normalized();
    r.setX( r.x() - nW );
    r.setY( r.y() - nW );
    r.setWidth( r.width() + nW );
    r.setHeight( r.height() + nW );
    // ready for next
    pointLast = point;

    return r;
}

//
// PFreeHandToolBar
//
PFreeHandToolBar::PFreeHandToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pLayout->addWidget( new PPenToolBar( this ) );
    pLayout->addStretch( 10 );
}

