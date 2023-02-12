#include "LibInfo.h"
#include "PGraphicsScene.h"

#include "PCanvas.h"

PGraphicsScene::PGraphicsScene( const QRectF &r, QObject *pObject )
    : QGraphicsScene( r, pObject )
{
    setBackgroundBrush( Qt::lightGray );

    pBackground = new PBackground();
    addItem( pBackground );

    pCanvas = new PCanvas();
    addItem( pCanvas );
}

PGraphicsScene::~PGraphicsScene()
{
}

void PGraphicsScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *pEvent )
{
    pCanvas->doDoubleClickEvent( pEvent );
}

void PGraphicsScene::mouseMoveEvent( QGraphicsSceneMouseEvent *pEvent )
{
    pCanvas->doMoveEvent( pEvent );
}

void PGraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent *pEvent )
{
    pCanvas->doPressEvent( pEvent );
}

void PGraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent *pEvent )
{
    pCanvas->doReleaseEvent( pEvent );
}



