#include "LibInfo.h"
#include "PDrawErase.h"

#include "PCanvas.h"
#include "PPenToolBar.h"

PDrawErase::PDrawErase( PCanvas *pCanvas )
    : PFreeBase( pCanvas )
{
}

QRect PDrawErase::doPress( QMouseEvent *pEvent )
{
    return doDrawShape( pEvent->pos() );
}

QRect PDrawErase::doMove( QMouseEvent *pEvent )
{
    return doDrawShape( pEvent->pos() );
}

QRect PDrawErase::doRelease( QMouseEvent *pEvent )
{
    return doDrawShape( pEvent->pos() );
}

QRect PDrawErase::doDrawShape( const QPoint &point )
{
    QImage *pImageCanvas = g_Context->getImage();
    PContextErase erase = g_Context->getErase();
    QRect rect( 0, 0, erase.size.width(), erase.size.height() );
    rect.moveCenter( point );

    // we can not use standard shape fill if transparent color because that will be a NoOp
    switch ( erase.nShape )
    {
    case PContextErase::ShapeRectangle:
        if ( pCanvas->isBackgroundTransparent() )
        {
            QColor colorBackground = pCanvas->getBackground();
            int nLeft = rect.left();
            int nRight = rect.right();
            int nTop = rect.top();
            int nBottom = rect.bottom();

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
            painter.setPen( Qt::NoPen );
            painter.drawRect( rect );
        }
        break;
    case PContextErase::ShapeEllipse:
        if ( pCanvas->isBackgroundTransparent() )
        {
            // create a mask where the area we want is black and the rest is transparent
            QColor colorMask( Qt::black );                         // assume black is for the area we want to keep
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
            doFill( rect, imageMask, pImageCanvas, colorMask );
        }
        else
        {
            QPainter painter( g_Context->getImage() );
            QBrush brush( pCanvas->getBackground() );
            brush.setStyle( Qt::SolidPattern );
            painter.setPen( Qt::NoPen );
            painter.drawEllipse( rect );
        }
        break;
    }

    return rect;
}

void PDrawErase::doFill( const QRect &rect, const QImage &imageMask, QImage *pImageCanvas, const QColor &colorMask )
{
    QColor colorBackground = pCanvas->getBackground();
    // fill (apply mask)
    int nLeft = rect.left();
    int nTop = rect.top();
    int nWidth = rect.width();
    int nHeight = rect.height();

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
}

