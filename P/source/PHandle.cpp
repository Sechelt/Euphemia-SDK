#include "LibInfo.h"
#include "PHandle.h"

#include "PCanvas.h"

PHandle::PHandle( PGraphicsView *pView, Type nType, const QPoint &pointCenter )
    : QWidget( pView->viewport() )
{
    this->pView = pView;
    this->nType = nType;

    resize( 16, 16 );
    setCenter( pointCenter ); 
}

void PHandle::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED( pEvent );

    QPainter painter( this );

    painter.setPen( QPen( Qt::NoPen ) );

    QBrush brush( Qt::green );
    brush.setStyle( Qt::SolidPattern );
    painter.setBrush( brush );

    painter.drawEllipse( rect() );

    switch ( nType )
    {
        case TypeDrag:
            {
                painter.drawImage( rect().topLeft(), QImage( ":P/Drag" ).scaled( rect().size() ) );
            }
            break;
        case TypeMovePoint:
            {
                brush.setColor( Qt::black );
                painter.setBrush( brush );

                QRect r;
                r.setWidth( rect().width() / 2 );
                r.setHeight( rect().height() / 2 );
                r.moveCenter( rect().center() );

                painter.drawEllipse( r );
            }
            break;
        case TypeSizeLeft:
            {
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeHoriz" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeRight:
            {
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeHoriz" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeTop:
            {
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeVert" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeBottom:
            {
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeVert" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeTopRight:
            {
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeTopRight" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeTopLeft:
            {
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeTopLeft" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeBottomRight:
            {
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeTopLeft" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeBottomLeft:
            {
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeTopRight" ).scaled( rect().size() ) );
            }
            break;
    }
}

void PHandle::setType( Type n )
{
    nType = n;
    update();
}

void PHandle::setCenter( const QPoint &pointCenter )
{
    QPoint pointPos( pointCenter.x() - rect().width() / 2, pointCenter.y() - rect().height() / 2 );
    move( pointPos );
}

void PHandle::doMoveBy( int nDeltaX, int nDeltaY )
{
    doMoveBy( QPoint( nDeltaX, nDeltaY ) );
}

void PHandle::doMoveBy( const QPoint &pointDelta )
{
    move( pos() + pointDelta );
}


