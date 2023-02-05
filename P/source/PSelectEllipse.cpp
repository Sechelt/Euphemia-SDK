#include "LibInfo.h"
#include "PSelectEllipse.h"

PSelectEllipse::PSelectEllipse( PCanvas *pCanvas )
    : PSelectRectangle( pCanvas )
{
}

void PSelectEllipse::doCut()
{
    // create a mask where the area we want is black and the rest is transparent
    QImage imageEllipse( r.size(), QImage::Format_ARGB32 );
    {
        imageEllipse.fill( Qt::transparent );
        QPainter painter( &imageEllipse );

        QBrush brush( Qt::black );
        brush.setStyle( Qt::SolidPattern );
        painter.setBrush( brush );

        painter.setPen( QPen( Qt::NoPen ) );

        painter.drawEllipse( QRect( 0, 0, r.width(), r.height() ) );
    }

    // make a copy (rect) and then trim to ellipse
    // put it into the system clipboard 
    QClipboard *pClipboard = QGuiApplication::clipboard();
    QImage image = g_Context->getImage()->copy( r );
    pClipboard->setImage( doTrim( image, imageEllipse, true ) );
}

void PSelectEllipse::doCopy()
{
    // create a mask where the area we want is black and the rest is transparent
    QImage imageEllipse( r.size(), QImage::Format_ARGB32 );
    {
        imageEllipse.fill( Qt::transparent );
        QPainter painter( &imageEllipse );

        QBrush brush( Qt::black );
        brush.setStyle( Qt::SolidPattern );
        painter.setBrush( brush );

        painter.setPen( QPen( Qt::NoPen ) );
        painter.drawEllipse( QRect( 0, 0, r.width(), r.height() ) );
    }

    // make a copy (rect) and then trim to ellipse
    // put it into the system clipboard 
     QClipboard *pClipboard = QGuiApplication::clipboard();
     QImage image = g_Context->getImage()->copy( r );
     pClipboard->setImage( doTrim( image, imageEllipse ) );
}

void PSelectEllipse::doPaint( QPainter *pPainter )
{
    //
    pPainter->setPen( QPen( Qt::DashLine ) );

    // paint
    pPainter->drawEllipse( r.normalized() );                         
}

QImage PSelectEllipse::doTrim( QImage &imageCopied, const QImage &imageMask, bool bErase )
{
    Q_ASSERT( imageCopied.size() == imageMask.size() );

    QColor      colorMask( Qt::black );                         // assume black is for the area we want to keep
    QColor      colorBackground( Qt::transparent );             // assume canvas background is transparent
    QImage *    pImageCanvas    = g_Context->getImage();
    int         nWidth          = imageCopied.size().width();
    int         nHeight         = imageCopied.size().height();

    // Seems that creating QColor with Qt::transparent does not properly set alpha channel so...
    colorBackground.setAlpha( 0 );

    for ( int nX = 0; nX < nWidth; nX++ )
    {
        for ( int nY = 0; nY < nHeight; nY++ )
        {
            // trim by setting pixel to mask trim color - probably transparent but definately not colorMask
            if ( imageMask.pixelColor( nX, nY ) != colorMask ) 
                imageCopied.setPixelColor( nX, nY, imageMask.pixelColor( nX, nY ) );
            // not in trim area so consider erasing (ie for a cut operation)
            else if ( bErase ) 
                pImageCanvas->setPixelColor( nX + r.x(), nY + r.y(), colorBackground );
        }
    }

    return imageCopied;
}


