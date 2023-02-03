#include "LibInfo.h"
#include "PDrawPolygon.h"

#include "PCanvas.h"
#include "PPenToolBar.h"

PDrawPolygon::PDrawPolygon( PCanvas *pCanvas )
    : PDrawPolyline( pCanvas )
{
}

/*!
 * \brief Paint the polygon. 
 *  
 * \author pharvey (2/1/23)
 * 
 * \param pPainter 
 * \param polygon 
 */
void PDrawPolygon::doPaint( QPainter *pPainter )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    // paint
    pPainter->drawPolygon( polygon );                         
}

//
// PPolygonToolBar
//
PPolygonToolBar::PPolygonToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pLayout->addWidget( new PPenToolBar( this ) );
    pLayout->addStretch( 10 );
}

