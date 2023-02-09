#include "LibInfo.h"
#include "PGraphicsScene.h"

PGraphicsScene::PGraphicsScene( PLayers *pLayers, const QRectF &r, QObject *pObject )
    : QGraphicsScene( r, pObject )
{
    setBackgroundBrush( Qt::lightGray );
    this->pLayers = pLayers;
    addWidget( pLayers );
}

PGraphicsScene::~PGraphicsScene()
{
}

PCanvas *PGraphicsScene::getCanvas()
{
    if ( !pLayers ) return nullptr;
    return pLayers->getCanvas();
}


