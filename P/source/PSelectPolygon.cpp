#include "LibInfo.h"
#include "PSelectPolygon.h"

PSelectPolygon::PSelectPolygon( PCanvas *pCanvas )
    : PDrawPolygon( pCanvas )
{
}

/*!
 * \brief Return a copy of the SELECTED AREA.
 * 
 * \author pharvey (2/6/23)
 * 
 * \return QImage 
 */
QImage PSelectPolygon::getCopy()
{
    QRect r = polygon.boundingRect();
    QImage imageCopy = g_Context->getImage()->copy( r );
    return getTrimmed( r, imageCopy, getMask() );
}

// remove the auto commit
void PSelectPolygon::doDoubleClick( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    if ( nState != StateDraw ) return;

    doManipulateState();
}

void PSelectPolygon::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );
    // no commit for a select shape - we just go straight to idle
    doIdleState();
}

void PSelectPolygon::doCut()
{
    QRect r = polygon.boundingRect();
    QClipboard *pClipboard = QGuiApplication::clipboard();
    QImage imageCopy = g_Context->getImage()->copy( r );
    pClipboard->setImage( getTrimmed( r, imageCopy, getMask(), true ) );
}

QImage PSelectPolygon::getMask()
{
    // create a mask where the area we want is black and the rest is transparent
    QRect r = polygon.boundingRect();
    QImage imageMask( r.size(), QImage::Format_ARGB32 );
    {
        QPolygonF polygonCopy = polygon;
        polygonCopy.translate( -r.left(), -r.top() );
        imageMask.fill( Qt::transparent );
        QPainter painter( &imageMask );

        QBrush brush( Qt::black );
        brush.setStyle( Qt::SolidPattern );
        painter.setBrush( brush );

        painter.setPen( QPen( Qt::NoPen ) );
        painter.drawPolygon( polygonCopy );
    }
    return imageMask;
}

void PSelectPolygon::doPaint( QPainter *pPainter )
{
    //
    pPainter->setPen( QPen( Qt::DashLine ) );

    // paint
    if ( nState == StateDraw )
    {
        QPolygon poly = polygon;
        poly.append( pointMouse );
        pPainter->drawPolygon( poly );                         
    }
    else
    {
        pPainter->drawPolygon( polygon );                         
    }
}



