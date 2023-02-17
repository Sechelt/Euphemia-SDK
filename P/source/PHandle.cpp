#include "LibInfo.h"
#include "PHandle.h"

#include "PCanvas.h"

PHandle::PHandle( PGraphicsView *pView, Type nType, const QPoint &pointCenter )
    : QWidget( pView->viewport() )
{
    this->pView = pView;
    this->nType = nType;

    resize( 12, 12 );
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

    switch ( nType )
    {
        case TypeDrag:
            {
                painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/Drag" ).scaled( rect().size() ) );
            }
            break;
        case TypeMovePoint:
            {
                painter.drawEllipse( rect() );
/*
                brush.setColor( Qt::black );       
                                                   
                QRect r;                           
                r.setWidth( rect().width() / 2 );  
                r.setHeight( rect().height() / 2 );
                r.moveCenter( rect().center() );   
                                                   
                painter.drawEllipse( r );          
*/
            }
            break;
        case TypeNewPoint:
            {
                painter.setBrush( Qt::yellow );

                QRect r;
                r.setWidth( rect().center().x() );
                r.setHeight( rect().center().y() );
                r.moveCenter( rect().center() );

                painter.drawEllipse( r );

                painter.setPen( QPen( Qt::black ) );
                painter.drawLine( r.center().x(), r.top(), r.center().x(), r.bottom() );
                painter.drawLine( r.left(), r.center().y(), r.right(), r.center().y() );
            }
            break;
        case TypeSizeLeft:
            {
                painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeHoriz" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeRight:
            {
                painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeHoriz" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeTop:
            {
                painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeVert" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeBottom:
            {
                painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeVert" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeTopRight:
            {
                painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeTopRight" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeTopLeft:
            {
                painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeTopLeft" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeBottomRight:
            {
                painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeTopLeft" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeBottomLeft:
            {
                painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeTopRight" ).scaled( rect().size() ) );
            }
            break;
        case TypeFillSeed:
            {
                QRect r( 0, 0, 4, 4 );
                r.moveCenter( rect().center() );
                painter.drawEllipse( r );

                painter.setPen( QPen( Qt::black ) );
                painter.drawLine( rect().topLeft(), rect().bottomRight() );
                painter.drawLine( rect().bottomLeft(), rect().topRight() );
            }
            break;
        case TypeFillStart:
            {
                painter.setBrush( QBrush( Qt::darkGreen ) );
                painter.drawEllipse( rect() );
            }
            break;
        case TypeFillStop:
            {
                painter.setBrush( QBrush( Qt::darkRed ) );
                painter.drawEllipse( rect() );
            }
            break;
        case TypeFillPoint:
            {
                painter.drawEllipse( rect() );
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


