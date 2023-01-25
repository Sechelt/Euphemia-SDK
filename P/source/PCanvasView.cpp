#include "LibInfo.h"
#include "PCanvasView.h"

PCanvasView::PCanvasView( QWidget *parent, PCanvas *pCanvas )
    : QScrollArea( parent )
{
    this->pCanvas = pCanvas;
//    setWidget( pCanvas );
}

/*
void PCanvasView::mousePressEvent( QMouseEvent *pEvent )
{
//    pCanvas->mousePressEvent( pEvent );
    QScrollArea::mousePressEvent( pEvent );
}

void PCanvasView::mouseMoveEvent( QMouseEvent *pEvent )
{
qDebug() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]";
    emit signalPos( pEvent->pos() );

//    pCanvas->mouseMoveEvent( pEvent );
    QScrollArea::mouseMoveEvent( pEvent );
}

void PCanvasView::mouseReleaseEvent( QMouseEvent *pEvent )
{
//    pCanvas->mouseReleaseEvent( pEvent );
    QScrollArea::mouseReleaseEvent( pEvent );
}
*/

