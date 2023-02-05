#include "PLayers.h"

//
// PLayers
//
PLayers::PLayers( QWidget *pParent, const QSize &size )
    : QWidget( pParent )
{
    pBackground = new PBackground( this );
    vectorCanvas.append( new PCanvas( this ) );
    resize( size );
}

void PLayers::resizeEvent( QResizeEvent *pEvent )
{
    QWidget::resizeEvent( pEvent );

    pBackground->resize( width(), height() );
    for ( PCanvas *pCanvas : vectorCanvas )
    {
        pCanvas->resize( width(), height() );
    }

}

//
// PLayersScrollArea
//
PLayersScrollArea::PLayersScrollArea( QWidget *parent )
    : QScrollArea( parent )
{
    pLayers = new PLayers( this );
    setWidget( pLayers );
}


