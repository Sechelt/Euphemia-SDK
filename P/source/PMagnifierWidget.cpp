#include "LibInfo.h"
#include "PMagnifierWidget.h"

#include "PContext.h"
#include "PCanvas.h"

PMagnifierWidget::PMagnifierWidget( QWidget *pParent )
    : QWidget( pParent )
{
    setMinimumSize( 100, 100 );
}

void PMagnifierWidget::setView( PGraphicsView *p )
{
    if ( pView )
    {
        disconnect( pView->getCanvas(), SIGNAL(signalMagnifierMoved()), this, SLOT(repaint()) );
    }

    pView = p;

    if ( pView )
    {
        connect( pView->getCanvas(), SIGNAL(signalMagnifierMoved()), this, SLOT(repaint()) );
    }
    update();
}

void PMagnifierWidget::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED(pEvent);

    PCanvas *       pCanvas = pView ? pView->getCanvas() : nullptr;
    PShapeBase *    pShape  = pCanvas ? pCanvas->getShape() : nullptr;

    QPainter painter( this );
    if ( !pShape || pCanvas->getTool() != PCanvas::ToolMagnifierSelection ) return;

    QImage *pImage = g_Context->getImage();

    int nXImage = pShape->getRect().topLeft().x();
    int nYImage = pShape->getRect().topLeft().y();
    int nSize = 16;
    int nXCells = rect().width() / nSize;
    int nYCells = rect().height() / nSize;

    if ( nXCells > pImage->width() - nXImage ) nXCells = pImage->width() - nXImage;
    if ( nYCells > pImage->height() - nYImage ) nYCells = pImage->height() - nYImage;

    QPen pen( Qt::white );
    painter.setPen( pen );
    for ( int nXCell = 0; nXCell < nXCells; nXCell++ )
    {
        for ( int nYCell = 0; nYCell < nYCells; nYCell++ )
        {
            painter.setBrush( pImage->pixelColor( nXImage + nXCell, nYImage + nYCell ) );
            painter.drawRect( nXCell * nSize, nYCell * nSize, nSize, nSize );
        }
    }
}



