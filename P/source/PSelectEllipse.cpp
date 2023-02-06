#include "LibInfo.h"
#include "PSelectEllipse.h"

PSelectEllipse::PSelectEllipse( PCanvas *pCanvas )
    : PSelectRectangle( pCanvas )
{
}

/*!
 * \brief Return a copy of the SELECTED AREA.
 * 
 * \author pharvey (2/6/23)
 * 
 * \return QImage 
 */
QImage PSelectEllipse::getCopy()
{
    QImage imageCopy = g_Context->getImage()->copy( r );
    return getTrimmed( r, imageCopy, getMask() );
}

void PSelectEllipse::doCut()
{
    QClipboard *pClipboard = QGuiApplication::clipboard();
    QImage imageCopy = g_Context->getImage()->copy( r );
    pClipboard->setImage( getTrimmed( r, imageCopy, getMask(), true ) );
}

void PSelectEllipse::doCopy()
{
    // put it into the system clipboard 
     QClipboard *pClipboard = QGuiApplication::clipboard();
     pClipboard->setImage( getCopy() );
}

QImage PSelectEllipse::getMask()
{
    // create a mask where the area we want is black and the rest is transparent
    QImage imageMask( r.size(), QImage::Format_ARGB32 );
    {
        imageMask.fill( Qt::transparent );
        QPainter painter( &imageMask );

        QBrush brush( Qt::black );
        brush.setStyle( Qt::SolidPattern );
        painter.setBrush( brush );

        painter.setPen( QPen( Qt::NoPen ) );
        painter.drawEllipse( QRect( 0, 0, r.width(), r.height() ) );
    }
    return imageMask;
}

void PSelectEllipse::doPaint( QPainter *pPainter )
{
    //
    pPainter->setPen( QPen( Qt::DashLine ) );

    // paint
    pPainter->drawEllipse( r.normalized() );                         
}



