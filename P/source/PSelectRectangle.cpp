#include "LibInfo.h"
#include "PSelectRectangle.h"

PSelectRectangle::PSelectRectangle( PCanvas *pCanvas, bool bAll )
    : PDrawRectangle( pCanvas )
{
    if ( bAll )
    {
        // fake doDraw to cover all
        r = QRect( QPoint( 0, 0 ), g_Context->getImage()->size() );
        nState = StateDraw;
        // go straight into manipulate
        doManipulate();
    }
}

/*!
 * \brief Return a copy of the SELECTED AREA.
 * 
 * \author pharvey (2/6/23)
 * 
 * \return QImage 
 */
QImage PSelectRectangle::getCopy()
{
    return g_Context->getImage()->copy( r );
}

// this removes the auto commit
QRect PSelectRectangle::doRelease( QMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    QRect rectUpdate;

    if ( pEvent->button() != Qt::LeftButton ) return rectUpdate;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        doManipulate();
        break;
    case StateManipulate:
        break;
    }

    return rectUpdate;
}

QRect PSelectRectangle::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );
    // no commit for a select shape - we just go straight to idle
    doIdle();
    return QRect();
}

void PSelectRectangle::doCut()
{
    QImage *pImage = g_Context->getImage();

    // copy
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setImage( pImage->copy( r ) );
    // delete (drawRect() with a transparent brush is a NoOp) so scan pixels
    QPainter painter( pImage );
    QColor colorBackground( Qt::transparent );      // this does not set alpha for some reason
    colorBackground.setAlpha( 0 );                  // so we set here
    r = r.normalized();
    int nLeft       = r.left() + 1;
    int nTop        = r.top() + 1;
    int nRight      = r.right() - 1;
    int nBottom     = r.bottom() - 1;

    for ( int nX = nLeft; nX <= nRight; nX++ )
    {
        for ( int nY = nTop; nY <= nBottom; nY++ )
        {
            pImage->setPixelColor( nX, nY, colorBackground );
        }
    }
}

void PSelectRectangle::doPaint( QPainter *pPainter )
{
    //
    pPainter->setPen( QPen( Qt::DashLine ) );

    // paint
    pPainter->drawRect( r.normalized() );                         
}

