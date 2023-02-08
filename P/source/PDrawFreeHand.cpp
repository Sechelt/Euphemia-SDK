#include "LibInfo.h"
#include "PDrawFreeHand.h"

#include "PCanvas.h"

PDrawFreeHand::PDrawFreeHand( PCanvas *pCanvas )
    : PFreeBase( pCanvas )
{
}

QRect PDrawFreeHand::doPress( QMouseEvent *pEvent )
{
    // init/reinit
    t = g_Context->getFreeHand();

    switch ( t.nShape )
    {
        case PContextFreeHand::ShapePen:
            pen = g_Context->getPen();
            brush.setStyle( Qt::NoBrush );
            break;
        case PContextFreeHand::ShapeEllipse:
        case PContextFreeHand::ShapeRectangle:
            pen.setStyle( Qt::NoPen );
            brush = g_Context->getBrush();
            break;
        case PContextFreeHand::ShapeCross:
            pen = g_Context->getPen();
            brush.setStyle( Qt::NoBrush );
            break;
        case PContextFreeHand::ShapeImageScaled:
            if ( t.image.isNull() )
                t.nShape = PContextFreeHand::ShapePen;
            else
                t.image = t.image.scaled( t.size );
            break;
        case PContextFreeHand::ShapeImage:
            if ( t.image.isNull() )
                t.nShape = PContextFreeHand::ShapePen;
            else
                t.size = t.image.size();
            break;
    }

    pointLast = pEvent->pos();
    return QRect();
}

QRect PDrawFreeHand::doMove( QMouseEvent *pEvent )
{
    return doDraw( pEvent->pos() );
}

QRect PDrawFreeHand::doRelease( QMouseEvent *pEvent )
{
    return doDraw( pEvent->pos() );
}

QRect PDrawFreeHand::doDraw( const QPoint &point )
{
    switch ( t.nShape )
    {
        case PContextFreeHand::ShapePen:
            return doDrawPen( point );
        case PContextFreeHand::ShapeEllipse:
            return doDrawEllipse( point );
        case PContextFreeHand::ShapeRectangle:
            return doDrawRectangle( point );
        case PContextFreeHand::ShapeCross:
            return doDrawCross( point );
        case PContextFreeHand::ShapeImageScaled:
        case PContextFreeHand::ShapeImage:
            return doDrawImage( point );
    }
    return doDrawPen( point );
}

QRect PDrawFreeHand::doDrawPen( const QPoint &point )
{
    // draw line
    QPainter painter( g_Context->getImage() );
    painter.setPen( pen );
    painter.drawLine( pointLast, point );
    // calc rect to be updated - factor in pen width to avoid artifacts
    int nW = pen.width();
    QRect r( pointLast, point );
    r = r.normalized();
    r.setX( r.x() - nW );
    r.setY( r.y() - nW );
    r.setWidth( r.width() + nW );
    r.setHeight( r.height() + nW );
    // ready for next
    pointLast = point;

    return r;
}

QRect PDrawFreeHand::doDrawEllipse( const QPoint &point )
{
    QRect r( point, t.size );
    r.moveCenter( point );

    // draw ellipse
    QPainter painter( g_Context->getImage() );
    painter.setPen( pen );
    painter.setBrush( brush );
    painter.drawEllipse( r );
    // ready for next
    pointLast = point;

    return r;
}

QRect PDrawFreeHand::doDrawRectangle( const QPoint &point )
{
    QRect r( point, t.size );
    r.moveCenter( point );

    // draw rectangle
    QPainter painter( g_Context->getImage() );
    painter.setPen( pen );
    painter.setBrush( brush );
    painter.drawRect( r );
    // ready for next
    pointLast = point;

    return r;
}

QRect PDrawFreeHand::doDrawCross( const QPoint &point )
{
    QRect r( point, t.size );
    r.moveCenter( point );

    // draw rectangle
    QPainter painter( g_Context->getImage() );
    painter.setPen( pen );
    painter.drawLine( r.left(), r.top() + r.height() / 2, r.right(), r.top() + r.height() / 2 );
    painter.drawLine( r.left() + r.width() / 2, r.top(), r.left() + r.width() / 2, r.bottom() );
    // calc rect to be updated - factor in pen width to avoid artifacts
    int nW = pen.width();
    r.setX( r.x() - nW );
    r.setY( r.y() - nW );
    r.setWidth( r.width() + nW );
    r.setHeight( r.height() + nW );
    // ready for next
    pointLast = point;

    return r;
}

QRect PDrawFreeHand::doDrawImage( const QPoint &point )
{
    QPainter painter( g_Context->getImage() );
    painter.drawImage( pointLast, t.image );
    // update last point even though we do not need it in this case
    pointLast = point;

    return QRect( point, t.size );
}

//
// PFreeHandToolBar
//
PFreeHandToolBar::PFreeHandToolBar( QWidget *p )
    : QWidget( p )
{
    PContextFreeHand t = g_Context->getFreeHand();
    QHBoxLayout *pLayout = new QHBoxLayout( this );

    pShape = new QComboBox( this );
    pShape->addItem( "Pen", PContextFreeHand::ShapePen );
    pShape->addItem( "Ellipse", PContextFreeHand::ShapeEllipse );
    pShape->addItem( "Rectangle", PContextFreeHand::ShapeRectangle );
    pShape->addItem( "Cross", PContextFreeHand::ShapeCross );
    pShape->addItem( "ImageScaled", PContextFreeHand::ShapeImageScaled );
    pShape->addItem( "Image", PContextFreeHand::ShapeImage );
    pShape->setCurrentIndex( pShape->findData( (int)t.nShape ) );
    pShape->setToolTip( tr("shape") );
    pLayout->addWidget( pShape );
    connect( pShape, SIGNAL(currentIndexChanged(int)), SLOT(slotShape(int)) );
    //
    pWidth = new QSpinBox( this );
    pWidth->setMinimum( 1 );
    pWidth->setMaximum( 50 );
    pWidth->setValue( t.size.width() );
    pWidth->setToolTip( tr("width") );
    pLayout->addWidget( pWidth );
    connect( pWidth, SIGNAL(valueChanged(int)), SLOT(slotWidth(int)) );
    //
    pHeight = new QSpinBox( this );
    pHeight->setMinimum( 1 );
    pHeight->setMaximum( 50 );
    pHeight->setValue( t.size.height() );
    pHeight->setToolTip( tr("height") );
    pLayout->addWidget( pHeight );
    connect( pHeight, SIGNAL(valueChanged(int)), SLOT(slotHeight(int)) );

    pImage = new WImageButton( t.image, this );
    pImage->setToolTip( tr("image (double click to change)") );
    pLayout->addWidget( pImage, 10 );
    connect( pImage, SIGNAL(signalClick()), SLOT(slotImage()) );

    pLayout->addStretch( 10 );

    connect( g_Context, SIGNAL(signalModified(const PContextFreeHand &)), SLOT(slotRefresh(const PContextFreeHand &)) );
}

void PFreeHandToolBar::slotRefresh( const PContextFreeHand &t )
{
    pShape->setCurrentIndex( pShape->findData( (int)t.nShape ) );
    pWidth->setValue( t.size.width() );
    pHeight->setValue( t.size.height() );
    pImage->setImage( t.image );
}

void PFreeHandToolBar::slotShape( int )
{
    PContextFreeHand t = g_Context->getFreeHand();
    t.nShape = PContextFreeHand::Shapes(pShape->currentData().toInt());
    g_Context->setFreeHand( t );
}

void PFreeHandToolBar::slotWidth( int )
{
    PContextFreeHand t = g_Context->getFreeHand();
    t.size.setWidth( pWidth->value() );
    g_Context->setFreeHand( t );
}

void PFreeHandToolBar::slotHeight( int )
{
    PContextFreeHand t = g_Context->getFreeHand();
    t.size.setHeight( pHeight->value() );
    g_Context->setFreeHand( t );
}

void PFreeHandToolBar::slotImage()
{
    PContextFreeHand t = g_Context->getFreeHand();
    t.image = pImage->getImage();
    g_Context->setFreeHand( t );
}

