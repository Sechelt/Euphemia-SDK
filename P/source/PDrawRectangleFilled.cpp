#include "LibInfo.h"
#include "PDrawRectangleFilled.h"

#include "PPenToolBar.h"
#include "PBrushToolBar.h"

PDrawRectangleFilled::PDrawRectangleFilled( PCanvas *pCanvas )
    : PDrawRectangle( pCanvas )
{
}

void PDrawRectangleFilled::doPaint( QPainter *pPainter )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setBrush( g_Context->getBrush() );

    // paint
    pPainter->drawRect( r.normalized() );                         
}

//
// PRectangleFilledToolBar
//
PRectangleFilledToolBar::PRectangleFilledToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pLayout->addWidget( new PPenToolBar( this ) );
    pLayout->addWidget( new PBrushToolBar( this ) );
    pLayout->addStretch( 10 );
}

