#include "LibInfo.h"
#include "PHandle.h"

#include "PCanvas.h"

PHandle::PHandle( PGraphicsView *pView, Type nType, const QPoint &pointCenter, Shape nShape, const QColor &color )
    : QWidget( pView->viewport() )
{
    this->pView     = pView;
    this->nType     = nType;
    this->nShape    = nShape;
    this->color     = color;

    resize( 12, 12 );
    setCenter( pointCenter ); 
}

void PHandle::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED( pEvent );

    QPainter painter( this );

    // defaults
    // - by default we draw a handle with the given pen/brush
    // - in some cases this is the handle, in other cases this is used as the background, in other cases it is ignored
    painter.setPen( QPen( Qt::black ) );
    painter.setBrush( QBrush( color ) );

    switch ( nType )
    {
        // these are default; shape, pen and brush and nothing more
        // - the diff types are still useful
        case TypeMovePoint:
        case TypeGradientIntermediate:
        case TypeGradientRadius:
        case TypeGradientFocal:
        case TypeGradientAngle:
            {
                if ( nShape == ShapeSquare )
                    painter.drawRect( rect() );
                else
                    painter.drawEllipse( rect() );
            }
            break;
        case TypeDrag:
            {
                if ( nShape == ShapeSquare )
                    painter.drawRect( rect() );
                else
                    painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/Drag" ).scaled( rect().size() ) );
            }
            break;
        case TypePointFactory:
            {
                painter.setBrush( Qt::yellow );

                QRect r;
                r.setWidth( rect().center().x() );
                r.setHeight( rect().center().y() );
                r.moveCenter( rect().center() );

                if ( nShape == ShapeSquare ) painter.drawRect( r );
                else painter.drawEllipse( r );

                painter.setPen( QPen( Qt::black ) );
                painter.drawLine( r.center().x(), r.top(), r.center().x(), r.bottom() );
                painter.drawLine( r.left(), r.center().y(), r.right(), r.center().y() );
            }
            break;
        case TypeSizeLeft:
            {
                if ( nShape == ShapeSquare ) painter.drawRect( rect() );
                else painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeHoriz" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeRight:
            {
                if ( nShape == ShapeSquare ) painter.drawRect( rect() );
                else painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeHoriz" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeTop:
            {
                if ( nShape == ShapeSquare ) painter.drawRect( rect() );
                else painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeVert" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeBottom:
            {
                if ( nShape == ShapeSquare ) painter.drawRect( rect() );
                else painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeVert" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeTopRight:
            {
                if ( nShape == ShapeSquare ) painter.drawRect( rect() );
                else painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeTopRight" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeTopLeft:
            {
                if ( nShape == ShapeSquare ) painter.drawRect( rect() );
                else painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeTopLeft" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeBottomRight:
            {
                if ( nShape == ShapeSquare ) painter.drawRect( rect() );
                else painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeTopLeft" ).scaled( rect().size() ) );
            }
            break;
        case TypeSizeBottomLeft:
            {
                if ( nShape == ShapeSquare ) painter.drawRect( rect() );
                else painter.drawEllipse( rect() );
                painter.drawImage( rect().topLeft(), QImage( ":P/ResizeTopRight" ).scaled( rect().size() ) );
            }
            break;
        case TypeFillSeed:
            {
                QRect r( 0, 0, 4, 4 );
                r.moveCenter( rect().center() );
                if ( nShape == ShapeSquare ) painter.drawRect( r );
                else painter.drawEllipse( r );

                painter.setPen( QPen( Qt::black ) );
                painter.drawLine( rect().topLeft(), rect().bottomRight() );
                painter.drawLine( rect().bottomLeft(), rect().topRight() );
            }
            break;
        case TypeGradientStart:
            {
                painter.setBrush( QBrush( Qt::darkGreen ) );
                if ( nShape == ShapeSquare ) painter.drawRect( rect() );
                else painter.drawEllipse( rect() );
            }
            break;
        case TypeGradientStop:
            {
                painter.setBrush( QBrush( Qt::darkRed ) );
                if ( nShape == ShapeSquare ) painter.drawRect( rect() );
                else painter.drawEllipse( rect() );
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


