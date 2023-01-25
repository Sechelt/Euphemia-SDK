#include "LibInfo.h"
#include "PHandle.h"

PHandle::PHandle( QWidget *pParent, Type nType, const QPoint &pointCenter )
    : QWidget( pParent )
{
    this->nType = nType;
    switch ( nType )
    {
        case TypeMove:
            {
                pen.setColor( Qt::black );
                pen.setWidth( 1 );

                brush.setColor( Qt::green );
                brush.setStyle( Qt::SolidPattern );

                QRect r( 0, 0, 11, 11 );
                r.moveCenter( pointCenter );
                setGeometry( r );
            }
            break;
        case TypeMovePoint:
            {
                pen.setColor( Qt::black );
                pen.setWidth( 1 );

                brush.setColor( Qt::blue );
                brush.setStyle( Qt::SolidPattern );

                QRect r( 0, 0, 11, 11 );
                r.moveCenter( pointCenter );
                setGeometry( r );
            }
            break;
        case TypeTopLeft:
        case TypeTopRight:
        case TypeBottomLeft:
        case TypeBottomRight:
        case TypeSizeH:
        case TypeSizeV:
        case TypeSizeHV:
            {
                pen.setColor( Qt::black );
                pen.setWidth( 1 );

                brush.setColor( Qt::lightGray );
                brush.setStyle( Qt::SolidPattern );

                QRect r( 0, 0, 11, 11 );
                r.moveCenter( pointCenter );
                setGeometry( r );
            }
            break;
    }
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
        case TypeMove:
        case TypeMovePoint:
            {
                QRect r = rect();
                r.setWidth( r.width() - pen.width() );
                r.setHeight( r.height() - pen.width() );
                painter.drawEllipse( r );
            }
            break;
        case TypeTopLeft:
        case TypeTopRight:
        case TypeBottomLeft:
        case TypeBottomRight:
        case TypeSizeH:
        case TypeSizeV:
        case TypeSizeHV:
            {
                QRect r = rect();
                r.setWidth( r.width() - pen.width() );
                r.setHeight( r.height() - pen.width() );
                painter.drawRect( r );
            }
            break;
    }
}


