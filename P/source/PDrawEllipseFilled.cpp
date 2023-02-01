#include "LibInfo.h"
#include "PDrawEllipseFilled.h"

#include "PPenToolBar.h"
#include "PBrushToolBar.h"

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

//
// PEllipseFilledToolBar
//
PEllipseFilledToolBar::PEllipseFilledToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pLayout->addWidget( new PPenToolBar( this ) );
    pLayout->addWidget( new PBrushToolBar( this ) );
    pLayout->addStretch( 10 );
}

