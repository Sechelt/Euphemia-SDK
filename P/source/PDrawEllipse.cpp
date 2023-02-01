#include "LibInfo.h"
#include "PDrawEllipse.h"

#include "PCanvas.h"
#include "PPenToolBar.h"

PDrawEllipse::PDrawEllipse( PCanvas *pCanvas, const QPoint &pointBegin )
    : PDrawRectangle( pCanvas, pointBegin )
{
}

void PDrawEllipse::doPaint( QPainter *pPainter, const QPoint &pointBegin, const QPoint &pointEnd )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setFont( g_Context->getFont() );
    // paint
    QRect r( pointBegin, pointEnd );
    r = r.normalized();
    pPainter->drawEllipse( r );                         
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

