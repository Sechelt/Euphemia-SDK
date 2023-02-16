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

    if ( nState == StateDraw )
    {
        QPolygon poly = polygon;
        poly.append( pointMouse );
        path.addPolygon( poly );
        pPainter->drawPolygon( poly, g_Context->getPolygonFilled().nFillRule );
    }
    else
    {
        path.addPolygon( polygon );
        pPainter->drawPolygon( polygon, g_Context->getPolygonFilled().nFillRule );
    }

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

