#include "LibInfo.h"
#include "PDrawPolygonFilled.h"

#include "PPenToolBar.h"
#include "PBrushToolBar.h"


//
// PPolygonFilledToolBar
//
PPolygonFilledToolBar::PPolygonFilledToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pLayout->addWidget( new PPenToolBar( this ) );
    pLayout->addWidget( new PBrushToolBar( this ) );
    pLayout->addStretch( 10 );
}

