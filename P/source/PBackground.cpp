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

#include "PBackground.h"

#include "PContext.h"

PBackground::PBackground()
    : QGraphicsObject()
{
}

void PBackground::paint( QPainter *pPainter, const QStyleOptionGraphicsItem *nOption, QWidget *pWidget )
{
    Q_UNUSED( nOption );
    Q_UNUSED( pWidget );

    QBrush brush = g_Context->getGeneral().brushTransparency;
    if ( brush.style() == Qt::NoBrush ) brush.setTextureImage( QImage(":W/Transparent") );
    pPainter->fillRect( 0, 0, boundingRect().width(), boundingRect().height(), brush );
}

QRectF PBackground::boundingRect() const
{
    if ( scene() ) return scene()->sceneRect();
    return QRectF();
}

