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

void PMagnifierWidget::mousePressEvent( QMouseEvent *pEvent )
{
    // init
    doCalcMatrix();
    if ( !pShape ) return pEvent->ignore();

    // get point in image (same as point in scene)
    QPoint pointCell = mapWidgetToCell( pEvent->pos() );
    if ( pointCell.x() >= nXCells ) return pEvent->ignore();
    if ( pointCell.y() >= nYCells ) return pEvent->ignore();

    QPoint pointScene = mapCellToScene( pointCell );
    if ( pointScene.x() >= pImage->width() ) return pEvent->ignore();
    if ( pointScene.y() >= pImage->height() ) return pEvent->ignore();

    // do it
    QPen pen = g_Context->getPen();

    pImage->setPixelColor( pointScene, pen.color() );

    // fini
    pCanvas->update();
    update();
}

void PMagnifierWidget::mouseMoveEvent( QMouseEvent *pEvent )
{
    if ( !pView ) return pEvent->ignore();
}

void PMagnifierWidget::mouseReleaseEvent( QMouseEvent *pEvent )
{
    if ( !pView ) return pEvent->ignore();
}

void PMagnifierWidget::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED(pEvent);

    // init
    doCalcMatrix();
    if ( !pShape ) return pEvent->ignore();

    QPainter painter( this );
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

void PMagnifierWidget::wheelEvent( QWheelEvent *pEvent )
{
    if ( !pView ) pEvent->ignore(); 

    QPoint pointPixels  = pEvent->pixelDelta();
    QPoint pointDegrees = pEvent->angleDelta() / 8;

    if ( !pointPixels.isNull() ) 
    {
        nSize += pointPixels.y() * 2; 
        if ( nSize > 48 ) nSize = 48;
        if ( nSize < 16 ) nSize = 16;
    } 
    else if ( !pointDegrees.isNull() ) 
    {
        QPoint pointSteps = pointDegrees / 15;
        nSize += pointSteps.y() * 2; 
        if ( nSize > 48 ) nSize = 48;
        if ( nSize < 16 ) nSize = 16;
    }

    pEvent->accept();
    update();
}

void PMagnifierWidget::doCalcMatrix()
{
    pCanvas = pView ? pView->getCanvas() : nullptr;

    // is canvas with mag tool current?
    if ( !pView || !pCanvas || pCanvas->getTool() != PCanvas::ToolMagnifierSelection )
    {
        pCanvas = nullptr;
        pShape  = nullptr;      // we can check this for null to see of rest is set
        pImage  = nullptr;
        nXImage = 0;
        nYImage = 0;
        nXCells = 0;
        nYCells = 0;
        return;
    }
    // is mag shape?
    pShape  = pCanvas->getShape();
    if ( !pShape ) return;

    // do it
    pImage  = g_Context->getImage();
    nXImage = pShape->getRect().topLeft().x();
    nYImage = pShape->getRect().topLeft().y();
    nXCells = rect().width() / nSize;
    nYCells = rect().height() / nSize;

    if ( nXCells > pImage->width() - nXImage ) nXCells = pImage->width() - nXImage;
    if ( nYCells > pImage->height() - nYImage ) nYCells = pImage->height() - nYImage;
}

QPoint PMagnifierWidget::mapCellToScene( const QPoint &pointCell )
{
    // can be out of range so caller should check
    return QPoint( nXImage + pointCell.x(), nYImage + pointCell.y() );
}

QPoint PMagnifierWidget::mapWidgetToCell( const QPoint &pointWidget )
{
    // can be out of range so caller should check
    return QPoint( pointWidget.x() / nSize, pointWidget.y() / nSize );
}


