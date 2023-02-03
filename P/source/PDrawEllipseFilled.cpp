#include "LibInfo.h"
#include "PDrawEllipseFilled.h"

#include "PPenToolBar.h"
#include "PBrushToolBar.h"

PDrawEllipseFilled::PDrawEllipseFilled( PCanvas *pCanvas )
    : PDrawEllipse( pCanvas )
{
}

void PDrawEllipseFilled::doPaint( QPainter *pPainter )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setBrush( g_Context->getBrush() );

    // paint
    pPainter->drawEllipse( r.normalized() );                         
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

