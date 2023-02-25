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
#include "PSelectPolygon.h"

PSelectPolygon::PSelectPolygon( PCanvas *pCanvas )
    : PDrawPolygon( pCanvas )
{
}

/*!
 * \brief Return a copy of the SELECTED AREA.
 * 
 * \author pharvey (2/6/23)
 * 
 * \return QImage 
 */
QImage PSelectPolygon::getCopy()
{
    QRect r = polygon.boundingRect();
    QImage imageCopy = g_Context->getImage()->copy( r );
    return getTrimmed( r, imageCopy, getMask() );
}

// remove the auto commit
void PSelectPolygon::doDoubleClick( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    if ( nState != StateDraw ) return;

    doManipulateState();
}

void PSelectPolygon::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );
    // no commit for a select shape - we just go straight to idle
    doIdleState();
}

void PSelectPolygon::doCut()
{
    QRect r = polygon.boundingRect();
    QClipboard *pClipboard = QGuiApplication::clipboard();
    QImage imageCopy = g_Context->getImage()->copy( r );
    pClipboard->setImage( getTrimmed( r, imageCopy, getMask(), true ) );
}

QImage PSelectPolygon::getMask()
{
    // create a mask where the area we want is black and the rest is transparent
    QRect r = polygon.boundingRect();
    QImage imageMask( r.size(), QImage::Format_ARGB32 );
    {
        QPolygonF polygonCopy = polygon;
        polygonCopy.translate( -r.left(), -r.top() );
        imageMask.fill( Qt::transparent );
        QPainter painter( &imageMask );

        QBrush brush( Qt::black );
        brush.setStyle( Qt::SolidPattern );
        painter.setBrush( brush );

        painter.setPen( QPen( Qt::NoPen ) );
        painter.drawPolygon( polygonCopy );
    }
    return imageMask;
}

void PSelectPolygon::doPaint( QPainter *pPainter, bool )
{
    //
    pPainter->setPen( QPen( Qt::DashLine ) );

    // paint
    if ( nState == StateDraw )
    {
        QPolygon poly = polygon;
        poly.append( pointMouse );
        pPainter->drawPolygon( poly );                         
    }
    else
    {
        pPainter->drawPolygon( polygon );                         
    }
}



