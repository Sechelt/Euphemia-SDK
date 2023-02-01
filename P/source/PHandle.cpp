#include "LibInfo.h"
#include "PHandle.h"

PHandle::PHandle( QWidget *pParent, Type nType, const QPoint &pointCenter )
    : QWidget( pParent )
{
    this->nType = nType;
    pen.setColor( Qt::black );
    pen.setWidth( 1 );

    brush.setColor( Qt::blue );
    brush.setStyle( Qt::SolidPattern );

    QRect r( 0, 0, 11, 11 );
    r.moveCenter( pointCenter );
    setGeometry( r );
}

void PHandle::setType( Type n )
{
    nType = n;
    update();
}

void PHandle::setCenter( const QPoint &pointCenter )
{
    move( pointCenter.x() - width() / 2, pointCenter.y() - height() / 2 );
}

void PHandle::doMoveBy( int nX, int nY )
{
    move( pos().x() + nX, pos().y() + nY );
}

void PHandle::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED( pEvent );

    QPainter painter( this );
    painter.setPen( pen );
    painter.setBrush( brush );

    switch ( nType )
    {
        case TypeDrag:
            {
                painter.drawImage( 0, 0, QImage( ":P/Drag" ).scaled( QSize( rect().width(), rect().height() ) ) );
            }
            break;
        case TypeMovePoint:
            {
                QRect r = rect();
                r.setWidth( r.width() - pen.width() );
                r.setHeight( r.height() - pen.width() );
                painter.drawEllipse( r );
            }
            break;
        case TypeSizeLeft:
            {
                painter.drawImage( 0, 0, QImage( ":P/ResizeHoriz" ).scaled( QSize( rect().width(), rect().height() ) ) );
            }
            break;
        case TypeSizeRight:
            {
                painter.drawImage( 0, 0, QImage( ":P/ResizeHoriz" ).scaled( QSize( rect().width(), rect().height() ) ) );
            }
            break;
        case TypeSizeTop:
            {
                painter.drawImage( 0, 0, QImage( ":P/ResizeVert" ).scaled( QSize( rect().width(), rect().height() ) ) );
            }
            break;
        case TypeSizeBottom:
            {
                painter.drawImage( 0, 0, QImage( ":P/ResizeVert" ).scaled( QSize( rect().width(), rect().height() ) ) );
            }
            break;
        case TypeSizeTopRight:
            {
                painter.drawImage( 0, 0, QImage( ":P/ResizeTopRight" ).scaled( QSize( rect().width(), rect().height() ) ) );
            }
            break;
        case TypeSizeTopLeft:
            {
                painter.drawImage( 0, 0, QImage( ":P/ResizeTopLeft" ).scaled( QSize( rect().width(), rect().height() ) ) );
            }
            break;
        case TypeSizeBottomRight:
            {
                painter.drawImage( 0, 0, QImage( ":P/ResizeTopLeft" ).scaled( QSize( rect().width(), rect().height() ) ) );
            }
            break;
        case TypeSizeBottomLeft:
            {
                painter.drawImage( 0, 0, QImage( ":P/ResizeTopRight" ).scaled( QSize( rect().width(), rect().height() ) ) );
            }
            break;
    }
}


