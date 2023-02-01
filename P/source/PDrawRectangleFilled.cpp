#include "LibInfo.h"
#include "PDrawRectangleFilled.h"

#include "PPenToolBar.h"
#include "PBrushToolBar.h"

PDrawRectangleFilled::PDrawRectangleFilled( PCanvas *pCanvas, const QPoint &pointBegin )
    : PDrawRectangle( pCanvas, pointBegin )
{
}

void PDrawRectangleFilled::doPaint( QPainter *pPainter, const QPoint &pointBegin, const QPoint &pointEnd )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setBrush( g_Context->getBrush() );

    // paint
    QRect r( pointBegin, pointEnd );
    r = r.normalized();
    pPainter->drawRect( r );                         
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

