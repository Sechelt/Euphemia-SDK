#include "PBackground.h"

#include "PContext.h"

PBackground::PBackground( QWidget *pParent )
    : QWidget( pParent )
{
}

void PBackground::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED(pEvent);

    QPainter painter( this );
    QBrush brush = g_Context->getGeneral().brushTransparency;
    if ( brush.style() == Qt::NoBrush ) brush.setTextureImage( QImage(":P/Transparent") );
    painter.fillRect(0, 0, width(), height(), brush );
}

