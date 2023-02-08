#include "LibInfo.h"
#include "PDrawPolygonFilled.h"

#include "PCanvas.h"

PDrawPolygonFilled::PDrawPolygonFilled( PCanvas *pCanvas )
    : PDrawPolygon( pCanvas )
{
}

void PDrawPolygonFilled::doPaint( QPainter *pPainter )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setBrush( g_Context->getBrush() );

    // paint
    QPainterPath path;
    path.addPolygon( polygon );

    // - Qt::OddEvenFill (default)
    // - Qt::WindingFill
    pPainter->drawPolygon( polygon, Qt::OddEvenFill );                         
    pPainter->fillPath( path, g_Context->getBrush() );
}

//
// PPolygonFilledToolBar
//
PPolygonFilledToolBar::PPolygonFilledToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pFillRule = new QComboBox( this );
    pFillRule->addItem( "Odd/Even Fill", Qt::OddEvenFill );
    pFillRule->addItem( "Winding Fill", Qt::WindingFill );
    pFillRule->setCurrentIndex( pFillRule->findData( g_Context->getPolygonFilled().nFillRule ) );
    pLayout->addWidget( pFillRule );
    connect( pFillRule, SIGNAL(activated(int)), SLOT(slotFillRule(int)) );

    pLayout->addStretch( 10 );

    connect( g_Context, SIGNAL(signalModified(const PContextPolygonFilled &)), SLOT(slotRefresh(const PContextPolygonFilled &)) );
}

void PPolygonFilledToolBar::slotRefresh( const PContextPolygonFilled &t )
{
    pFillRule->setCurrentIndex( pFillRule->findData( t.nFillRule ) );
}

void PPolygonFilledToolBar::slotFillRule( int n )
{
    PContextPolygonFilled polygonfilled = g_Context->getPolygonFilled();
    polygonfilled.nFillRule = (Qt::FillRule)pFillRule->itemData( n ).toInt();
    g_Context->setPolygonFilled( polygonfilled );
}

