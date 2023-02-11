#include "LibInfo.h"
#include "PHandle.h"

PHandle::PHandle( Type nType, const QPointF &pointCenter )
{
    this->nType = nType;

    r.setSize( QSizeF( 16, 16 ) );
    r.moveCenter( pointCenter ); 
}

void PHandle::paint( QPainter *pPainter, const QStyleOptionGraphicsItem *nOption, QWidget *pWidget )
{
    Q_UNUSED( nOption );
    Q_UNUSED( pWidget );

    pPainter->setPen( QPen( Qt::NoPen ) );

    QBrush brush( Qt::green );
    brush.setStyle( Qt::SolidPattern );
    pPainter->setBrush( brush );

    pPainter->drawEllipse( r );

    switch ( nType )
    {
        case TypeDrag:
            {
                pPainter->drawImage( r.topLeft(), QImage( ":P/Drag" ).scaled( r.size().toSize() ) );
            }
            break;
        case TypeMovePoint:
            {
                brush.setColor( Qt::black );
                pPainter->setBrush( brush );

                QRectF rect;
                rect.setWidth( r.width() / 2 );
                rect.setHeight( r.height() / 2 );
                rect.moveCenter( r.center() );

                pPainter->drawEllipse( rect );
            }
            break;
        case TypeSizeLeft:
            {
                pPainter->drawImage( r.topLeft(), QImage( ":P/ResizeHoriz" ).scaled( r.size().toSize() ) );
            }
            break;
        case TypeSizeRight:
            {
                pPainter->drawImage( r.topLeft(), QImage( ":P/ResizeHoriz" ).scaled( r.size().toSize() ) );
            }
            break;
        case TypeSizeTop:
            {
                pPainter->drawImage( r.topLeft(), QImage( ":P/ResizeVert" ).scaled( r.size().toSize() ) );
            }
            break;
        case TypeSizeBottom:
            {
                pPainter->drawImage( r.topLeft(), QImage( ":P/ResizeVert" ).scaled( r.size().toSize() ) );
            }
            break;
        case TypeSizeTopRight:
            {
                pPainter->drawImage( r.topLeft(), QImage( ":P/ResizeTopRight" ).scaled( r.size().toSize() ) );
            }
            break;
        case TypeSizeTopLeft:
            {
                pPainter->drawImage( r.topLeft(), QImage( ":P/ResizeTopLeft" ).scaled( r.size().toSize() ) );
            }
            break;
        case TypeSizeBottomRight:
            {
                pPainter->drawImage( r.topLeft(), QImage( ":P/ResizeTopLeft" ).scaled( r.size().toSize() ) );
            }
            break;
        case TypeSizeBottomLeft:
            {
                pPainter->drawImage( r.topLeft(), QImage( ":P/ResizeTopRight" ).scaled( r.size().toSize() ) );
            }
            break;
    }
}

/*!
 * \brief Scene, or a PShapeBase object, wants to know our geometry. 
 *  
 * A \sa PShapeBase object will use this to determine if the mouse has been 
 * clicked on one of its a handles. 
 *  
 * \author pharvey (2/10/23)
 * 
 * \return QRectF 
 */
QRectF PHandle::boundingRect() const
{
    return r;
}

void PHandle::setType( Type n )
{
    nType = n;
    update();
}

void PHandle::setCenter( const QPointF &pointCenter )
{
    r.moveCenter( pointCenter );
}

void PHandle::doMoveBy( qreal nX, qreal nY )
{
    doMoveBy( QPointF( nX, nY ) );
}

void PHandle::doMoveBy( const QPointF &pointDelta )
{
    r.moveCenter( r.center() - pointDelta );
}


