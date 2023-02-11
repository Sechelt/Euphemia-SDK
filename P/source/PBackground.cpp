#include "PBackground.h"

#include "PContext.h"

PBackground::PBackground()
    : QGraphicsObject()
{
}

void PBackground::paint( QPainter *pPainter, const QStyleOptionGraphicsItem *nOption, QWidget *pWidget )
{
    Q_UNUSED( nOption );
    Q_UNUSED( pWidget );

    QBrush brush = g_Context->getGeneral().brushTransparency;
    if ( brush.style() == Qt::NoBrush ) brush.setTextureImage( QImage(":W/Transparent") );
    pPainter->fillRect( 0, 0, boundingRect().width(), boundingRect().height(), brush );
}

QRectF PBackground::boundingRect() const
{
    if ( scene() ) return scene()->sceneRect();
    return QRectF();
}

