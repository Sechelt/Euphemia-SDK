/*
 * Copyright 2022-2023, Peter Harvey <pharvey@codebydesign.com>
 *
 * This file is part of Euphemia-SDK.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "LibInfo.h"
#include "PGraphicsView.h"

#include "PGraphicsScene.h"
#include "PCanvas.h"

PGraphicsView::PGraphicsView( PGraphicsScene *pScene, QWidget *pParent )
    : QGraphicsView( pScene, pParent )
{
    Q_ASSERT(pScene);                                                                                       
    // \todo only way to catch this?                                                                        
    // - when document size changes and not triggering other events because all is still in viewport        
    // connect( pScene, SIGNAL(sceneRectChanged(const QRectF &)), SLOT(slotSceneRectChanged(const QRectF &)) );

#if QT_VERSION < 0x060000
    setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing );
#else
    setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform );
#endif
    setAlignment( Qt::AlignLeft | Qt::AlignTop );
}

PGraphicsScene *PGraphicsView::getScene()
{
    QGraphicsScene *p = scene();
    if ( !p ) 
    {
        qWarning("View has no scene.");
        return nullptr;
    }
    Q_ASSERT( p->inherits( "PGraphicsScene" ) );
    return (PGraphicsScene*)p;
}

PCanvas *PGraphicsView::getCanvas()
{
    PGraphicsScene *p = getScene();
    if ( !p ) 
    {
        qWarning("View has no scene.");
        return nullptr;
    }
    return p->getCanvas();
}

qreal PGraphicsView::getScale()
{
    return transform().m11();
}

void PGraphicsView::slotZoomChanged( WZoomWidget::FitTypes nFit, int n )
{
    if ( nFit == nZoomFit && n == nZoom ) return;

    nZoomFit    = nFit;
    nZoom       = n;

    // get scale
    switch ( nFit )
    {
    case WZoomWidget::FitWidth:
        break;
    case WZoomWidget::FitHeight:
        break;
    case WZoomWidget::FitAll:
        break;
    case WZoomWidget::FitIgnore:
        break;
    }

    qreal nScale = qreal(nZoom) / 100;

    // apply scale
    setScale( nScale );
    emit signalZoomChanged( nFit, nZoom );
}

void PGraphicsView::setScale( qreal n )
{
    if ( n == getScale() ) return;

    QTransform transform;
    transform.scale( n, n );  
    setTransform( transform );

    getScene()->getCanvas()->doZoomChanged();
}


