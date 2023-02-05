#include "PBackground.h"

PBackground::PBackground( QWidget *pParent )
    : QWidget( pParent )
{
    brush.setTextureImage( QImage( ":P/Transparent" ) );
}

void PBackground::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED(pEvent);
// qInfo() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ <<"]";

    QPainter painter( this );
    painter.fillRect( 0, 0, width(), height(), brush );
}

