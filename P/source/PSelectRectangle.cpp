#include "LibInfo.h"
#include "PSelectRectangle.h"

PSelectRectangle::PSelectRectangle( PCanvas *pCanvas )
    : PDrawRectangle( pCanvas )
{
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

void PSelectRectangle::doCopy()
{
     QClipboard *clipboard = QGuiApplication::clipboard();
     clipboard->setImage( g_Context->getImage()->copy( r ) );
}

bool PSelectRectangle::canCommit()
{
    return false;
}

bool PSelectRectangle::canCut()
{
    if ( nState == StateManipulate ) return true;
    return false;
}

bool PSelectRectangle::canCopy()
{
    if ( nState == StateManipulate ) return true;
    return false;
}

void PSelectRectangle::doPaint( QPainter *pPainter )
{
    //
    pPainter->setPen( QPen( Qt::DashLine ) );

    // paint
    pPainter->drawRect( r.normalized() );                         
}

