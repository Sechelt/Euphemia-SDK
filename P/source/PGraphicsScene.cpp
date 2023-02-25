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



