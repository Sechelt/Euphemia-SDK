#include "LibInfo.h"
#include "PFillFlood.h"

#include "PBrushToolBar.h"

PFillFloodToolBar::PFillFloodToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pLayout->addWidget( new PBrushToolBar( this ) );
    pLayout->addStretch( 10 );
}

