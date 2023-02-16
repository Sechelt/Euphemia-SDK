#include "LibInfo.h"
#include "PDrawErase.h"

#include "PCanvas.h"
#include "PPenToolBar.h"

PDrawErase::PDrawErase( PCanvas *pCanvas )
    : PFreeBase( pCanvas )
{
}

void PDrawErase::doPress( PMouseEvent *pEvent )
{
    // init/reinit

    // set color fully transparent (Qt::transparent is not quite fully transparent)
    colorTransparent.setRgba( qRgba( 0, 0, 0, 0 ) ); 
    // we just want the width for the pen
    pen.setWidth( g_Context->getPen().width() );
    pen.setStyle( Qt::SolidLine );
    pen.setColor( colorTransparent ); 
    // make copy - include any image - which we can scale without altering context
    t = g_Context->getErase();

    switch ( t.nShape )
    {
        case PContextErase::ShapePen:
        case PContextErase::ShapeEllipse:
        case PContextErase::ShapeRectangle:
        case PContextErase::ShapeCross:
            break;
        case PContextErase::ShapeImageScaled:
            if ( t.image.isNull() )
                t.nShape = PContextErase::ShapePen;
            else
            {
                t.image = t.image.scaled( t.size );
                getMask( &t.image );
            }
            break;
        case PContextErase::ShapeImage:
            if ( t.image.isNull() )
                t.nShape = PContextErase::ShapePen;
            else
            {
                t.size = t.image.size();
                getMask( &t.image );
            }
            break;
    }

    pointLast = pEvent->pos();
}

void PDrawErase::doMove( PMouseEvent *pEvent )
{
    doErase( pEvent->pos() );
}

void PDrawErase::doRelease( PMouseEvent *pEvent )
{
    doErase( pEvent->pos() );
}

/*!
 * \brief Erase using the current 'shape'. 
 *  
 * This will erase to PCanvas::getBackground() or transparent. 
 *  
 * The least efficient is to erase to transparent because drawing 
 * with QPen/QBrush with Qt::Transparent (or Alpha=0) is a NoOp. 
 * To get around this we often resort to creating and applying a mask. 
 * 
 * \author pharvey (2/16/23)
 * 
 * \param point  
 */
void PDrawErase::doErase( const QPoint &point )
{
    switch ( t.nShape )
    {
        case PContextErase::ShapePen:
            doErasePen( point );
            break;
        case PContextErase::ShapeEllipse:
            doEraseEllipse( point );
            break;
        case PContextErase::ShapeRectangle:
            doEraseRectangle( point );
            break;
        case PContextErase::ShapeCross:
            doEraseCross( point );
            break;
        case PContextErase::ShapeImageScaled:
        case PContextErase::ShapeImage:
            doEraseImage( point );
            break;
    }
    pCanvas->update();
}

/*!
 * \brief Erase using the current pen. 
 *  
 * This will use the current pen width and style but not the color nor 
 * any brush that may be in the pen. 
 *  
 * The main difference from an doEraseEllipse is that; 
 *  
 * - this will be painted by drawing a line between last and current point (not stamp) 
 * - pen is, mostly, a circle 
 * - this will use pen style (although SolidLine is probably most useful)
 *  
 * \author pharvey (2/16/23)
 * 
 * \param point  
 */
void PDrawErase::doErasePen( const QPoint &point )
{
    // draw line
    if ( pCanvas->isBackgroundTransparent() )
    {
        // create a mask

        // mask geometry
        QRect rect( pointLast, point );
        rect = rect.normalized();
        // line in mask coords
        QPoint pointBegin = pointLast - rect.topLeft();
        QPoint pointEnd = point - rect.topLeft();
        // mask image
        QImage imageMask( rect.size(), QImage::Format_ARGB32 );
        imageMask.fill( Qt::white );            // white will be ignored
        // paint mask
        QPainter painter( &imageMask );
        QPen pen = g_Context->getPen();
        pen.setColor( Qt::black );              // black will indicate pixel to erase
        painter.setPen( pen );
        painter.drawLine( pointBegin, pointEnd );
        // apply mask
        doErase( rect, imageMask, g_Context->getImage(), QColor( Qt::black ) );
    }
    else
    {
        QPainter painter( g_Context->getImage() );
        QPen pen = g_Context->getPen();
        pen.setColor( pCanvas->getBackground() );
        painter.setPen( pen );
        painter.drawLine( pointLast, point );
    }
    // ready for next
    pointLast = point;
}

/*!
 * \brief Erase using an ellipse.
 * 
 * \author pharvey (2/16/23)
 * 
 * \param point  
 */
void PDrawErase::doEraseEllipse( const QPoint &point )
{
    QImage *pImageCanvas = g_Context->getImage();               
    QRect rect( 0, 0, t.size.width(), t.size.height() );
    rect.moveCenter( point );                                   

    if ( pCanvas->isBackgroundTransparent() )
    {
        // create a mask
        QColor colorMask( Qt::black );
        QImage imageMask( rect.size(), QImage::Format_ARGB32 );
        {
            imageMask.fill( Qt::transparent );
            QPainter painter( &imageMask );

            QBrush brush( colorMask );
            brush.setStyle( Qt::SolidPattern );
            painter.setBrush( brush );

            painter.setPen( QPen( Qt::NoPen ) );

            painter.drawEllipse( QRect( 0, 0, rect.width(), rect.height() ) );
        }
        doErase( rect, imageMask, pImageCanvas, colorMask );
    }
    else
    {
        QPainter painter( g_Context->getImage() );
        QBrush brush( pCanvas->getBackground() );
        brush.setStyle( Qt::SolidPattern );
        painter.setBrush( brush );
        painter.setPen( Qt::NoPen );
        painter.drawEllipse( rect );
    }
}

/*!
 * \brief Erase using a rectangle.
 * 
 * \author pharvey (2/16/23)
 * 
 * \param point  
 */
void PDrawErase::doEraseRectangle( const QPoint &point )
{
    QImage *pImageCanvas = g_Context->getImage();               
    QRect rect( 0, 0, t.size.width(), t.size.height() );
    rect.moveCenter( point );                                   

    if ( pCanvas->isBackgroundTransparent() )
    {
        QColor colorBackground = pCanvas->getBackground();
        int nLeft     = rect.left();
        int nRight    = rect.right();
        int nTop      = rect.top();
        int nBottom   = rect.bottom();

        // do not need a mask for rectangle
        for ( int nX = nLeft; nX <= nRight; nX++ )
        {
            for ( int nY = nTop; nY <= nBottom; nY++ )
            {
                pImageCanvas->setPixelColor( nX, nY, colorBackground );
            }
        }
    }
    else
    {
        QPainter painter( g_Context->getImage() );
        QBrush brush( pCanvas->getBackground() );
        brush.setStyle( Qt::SolidPattern );
        painter.setBrush( brush );
        painter.setPen( Qt::NoPen );
        painter.drawRect( rect );
    }
}

/*!
 * \brief Erase using a cross. 
 *  
 * A cross can be useful when creating granular/pixelated images. 
 *  
 * \todo Factor in line width to prevent wonky looking cross with think line. 
 *  
 * \author pharvey (2/16/23)
 * 
 * \param point  
 */
void PDrawErase::doEraseCross( const QPoint &point )
{
    if ( pCanvas->isBackgroundTransparent() )
    {
        // create a mask

        QRect rect( QPoint( 0, 0 ), t.size );
        QRect rectGeometry = rect;
        rectGeometry.moveCenter( point );

        // mask image
        QImage imageMask( rect.size(), QImage::Format_ARGB32 );
        imageMask.fill( Qt::white );            // white will be ignored
        // paint mask
        QPainter painter( &imageMask );
        QPen pen( Qt::black );                  // black will indicate pixel to erase
        pen.setWidth( g_Context->getPen().width() );
        painter.setPen( pen );
        painter.drawLine( 0, rect.center().y(), rect.width(), rect.center().y() );
        painter.drawLine( rect.center().x(), 0, rect.center().x(), rect.height() );

        // apply mask
        doErase( rectGeometry, imageMask, g_Context->getImage(), QColor(Qt::black) );
    }
    else
    {
        QRect r( point, t.size );
        r.moveCenter( point );
        // paint a cross in the background color
        QPainter painter( g_Context->getImage() );
        QPen pen( pCanvas->getBackground() );
        pen.setWidth( g_Context->getPen().width() );
        painter.setPen( pen );
        painter.drawLine( r.left(), point.y(), r.right(), point.y() );
        painter.drawLine( point.x(), r.top(), point.x(), r.bottom() );
    }
    // ready for next
    pointLast = point;
}

/*!
 * \brief Uses a given image as a mask. 
 *  
 * All colors (other than transparent) are converted to black to create a mask. 
 * This mask is then passed to doErase to apply to canvas image. 
 *  
 * The mask can be scaled (to erase size) or not. 
 *  
 * \note The t.image is preprocessed in doPress for efficiency. 
 *  
 * \author pharvey (2/16/23)
 * 
 * \param point  
 */
void PDrawErase::doEraseImage( const QPoint &point )
{
    // t.image should be preprocessed for us in doPress

    if ( t.image.isNull() )
    {
        qWarning( "Null image." );
        return;
    }

    QRect rectGeometry( QPoint( 0, 0 ), t.image.size() );
    rectGeometry.moveCenter( point );

    // apply mask
    doErase( rectGeometry, t.image, g_Context->getImage(), QColor( Qt::black ) );
}

/*!
 * \brief Modifies the given image to make it a mask. 
 *  
 * All color pixels (pixels that are not transparent) are set to black. 
 * 
 * \author pharvey (2/16/23)
 * 
 * \param pImage 
 * 
 * \return QImage* 
 */
QImage *PDrawErase::getMask( QImage *pImage )
{
    if ( pImage->isNull() ) return pImage;

    int nWidth = pImage->size().width();
    int nHeight = pImage->size().height();

    for ( int nX = 0; nX < nWidth; nX++ )
    {
        for ( int nY = 0; nY < nHeight; nY++ )
        {
            QColor color = pImage->pixelColor( nX, nY );
            if ( color != Qt::transparent && color.alpha() != 0 ) 
            {
                pImage->setPixelColor( nX, nY, Qt::black );
            }
        }
    }

    return pImage;
}


/*!
 * \brief Applies a mask to the canvas image. 
 *  
 * Scans the mask. Wherever it finds a colorMask pixel it sets the corresponding pixel in the 
 * canvas image to the background color. 
 *  
 * \note    Setting pen or brush color to Qt::transparent is a NoOp even if Alpha is
 *          explicitly set to 0. So we must set image pixels to transparent directly. 
 *  
 * \author pharvey (2/16/23)
 * 
 * \param rect         
 * \param imageMask    
 * \param pImageCanvas 
 * \param colorMask    
 */
void PDrawErase::doErase( const QRect &rect, const QImage &imageMask, QImage *pImageCanvas, const QColor &colorMask )
{
    QColor colorBackground = pCanvas->getBackground();
    // fill (apply mask)
    int nLeft     = rect.left();
    int nTop      = rect.top();
    int nWidth    = rect.width();
    int nHeight   = rect.height();

    // process the mask
    for ( int nX = 0; nX < nWidth; nX++ )
    {
        for ( int nY = 0; nY < nHeight; nY++ )
        {
            if ( imageMask.pixelColor( nX, nY ) == colorMask ) 
            {
                pImageCanvas->setPixelColor( nX + nLeft, nY + nTop, colorBackground );
            }
        }
    }
}

//
// PEraseToolBar
//
PEraseToolBar::PEraseToolBar( QWidget *p )
    : QWidget( p )
{
    PContextErase t = g_Context->getErase();
    QHBoxLayout *pLayout = new QHBoxLayout( this );

    pShape = new QComboBox( this );
    pShape->addItem( "Pen", PContextErase::ShapePen );
    pShape->addItem( "Ellipse", PContextErase::ShapeEllipse );
    pShape->addItem( "Rectangle", PContextErase::ShapeRectangle );
    pShape->addItem( "Cross", PContextErase::ShapeCross );
    pShape->addItem( "ImageScaled", PContextErase::ShapeImageScaled );
    pShape->addItem( "Image", PContextErase::ShapeImage );
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

    pLayout->addStretch( 16 );

    connect( g_Context, SIGNAL(signalModified(const PContextErase &)), SLOT(slotRefresh(const PContextErase &)) );
}

void PEraseToolBar::slotRefresh( const PContextErase &t )
{
    pShape->setCurrentIndex( pShape->findData( (int)t.nShape ) );
    pWidth->setValue( t.size.width() );
    pHeight->setValue( t.size.height() );
    pImage->setImage( t.image );
}

void PEraseToolBar::slotShape( int )
{
    PContextErase t = g_Context->getErase();
    t.nShape = PContextErase::Shapes(pShape->currentData().toInt());
    g_Context->setErase( t );
}

void PEraseToolBar::slotWidth( int )
{
    PContextErase t = g_Context->getErase();
    t.size.setWidth( pWidth->value() );
    g_Context->setErase( t );
}

void PEraseToolBar::slotHeight( int )
{
    PContextErase t = g_Context->getErase();
    t.size.setHeight( pHeight->value() );
    g_Context->setErase( t );
}

void PEraseToolBar::slotImage()
{
    QImage image;

    QString stringFileName = QFileDialog::getOpenFileName( this, tr("Select image..."), QString(), "Image files (*.png *.xpm *.jpg)" );
    if ( !stringFileName.isEmpty() )
    {
        image.load( stringFileName );
    }

    //
    PContextErase t = g_Context->getErase();
    t.image = image;
    g_Context->setErase( t );
}

