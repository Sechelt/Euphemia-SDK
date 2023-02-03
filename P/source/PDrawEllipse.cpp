#include "LibInfo.h"
#include "PDrawEllipse.h"

#include "PCanvas.h"
#include "PPenToolBar.h"

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

//
// PEllipseToolBar
//
PEllipseToolBar::PEllipseToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pLayout->addWidget( new PPenToolBar( this ) );
    pLayout->addStretch( 10 );
}

