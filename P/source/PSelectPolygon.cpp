#include "LibInfo.h"
#include "PSelectPolygon.h"

PSelectPolygon::PSelectPolygon( PCanvas *pCanvas )
    : PDrawPolygon( pCanvas )
{
}

QRect PSelectPolygon::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );
    // no commit for a select shape - we just go straight to idle
    doIdle();
    return QRect();
}

void PSelectPolygon::doCut()
{
    QRect r = polygon.boundingRect();
    // create a mask where the area we want is black and the rest is transparent
    QImage imagePolygon( r.size(), QImage::Format_ARGB32 );
    {
        QPolygon polygonCopy = polygon;
        polygonCopy.translate( -r.left(), -r.top() );
        imagePolygon.fill( Qt::transparent );
        QPainter painter( &imagePolygon );

        QBrush brush( Qt::black );
        brush.setStyle( Qt::SolidPattern );
        painter.setBrush( brush );

        painter.setPen( QPen( Qt::NoPen ) );
        painter.drawPolygon( polygonCopy );
    }

    // make a copy (rect) and then trim to polygon
    // put it into the system clipboard 
     QClipboard *pClipboard = QGuiApplication::clipboard();
     QImage image = g_Context->getImage()->copy( r );
     pClipboard->setImage( doTrim( image, imagePolygon, true ) );
}

void PSelectPolygon::doCopy()
{
    QRect r = polygon.boundingRect();
    // create a mask where the area we want is black and the rest is transparent
    QImage imagePolygon( r.size(), QImage::Format_ARGB32 );
    {
        QPolygon polygonCopy = polygon;
        polygonCopy.translate( -r.left(), -r.top() );
        imagePolygon.fill( Qt::transparent );
        QPainter painter( &imagePolygon );

        QBrush brush( Qt::black );
        brush.setStyle( Qt::SolidPattern );
        painter.setBrush( brush );

        painter.setPen( QPen( Qt::NoPen ) );
        painter.drawPolygon( polygonCopy );
    }

    // make a copy (rect) and then trim to polygon
    // put it into the system clipboard 
     QClipboard *pClipboard = QGuiApplication::clipboard();
     QImage image = g_Context->getImage()->copy( r );
     pClipboard->setImage( doTrim( image, imagePolygon ) );
}

bool PSelectPolygon::canCommit()
{
    return false;
}

bool PSelectPolygon::canCut()
{
    if ( nState == StateManipulate ) return true;
    return false;
}

bool PSelectPolygon::canCopy()
{
    if ( nState == StateManipulate ) return true;
    return false;
}

void PSelectPolygon::doPaint( QPainter *pPainter )
{
    //
    pPainter->setPen( QPen( Qt::DashLine ) );

    // paint
    pPainter->drawPolygon( polygon );                         
}

QImage PSelectPolygon::doTrim( QImage &imageCopied, const QImage &imageMask, bool bErase )
{
    Q_ASSERT( imageCopied.size() == imageMask.size() );

    QRect       r               = polygon.boundingRect();
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


