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
#include "PSelectEllipse.h"

PSelectEllipse::PSelectEllipse( PCanvas *pCanvas )
    : PSelectRectangle( pCanvas )
{
}

/*!
 * \brief Return a copy of the SELECTED AREA.
 * 
 * \author pharvey (2/6/23)
 * 
 * \return QImage 
 */
QImage PSelectEllipse::getCopy()
{
    QImage imageCopy = g_Context->getImage()->copy( r );
    return getTrimmed( r, imageCopy, getMask() );
}

void PSelectEllipse::doCut()
{
    QClipboard *pClipboard = QGuiApplication::clipboard();
    QImage imageCopy = g_Context->getImage()->copy( r );
    pClipboard->setImage( getTrimmed( r, imageCopy, getMask(), true ) );
}

void PSelectEllipse::doCopy()
{
    // put it into the system clipboard 
     QClipboard *pClipboard = QGuiApplication::clipboard();
     pClipboard->setImage( getCopy() );
}

QImage PSelectEllipse::getMask()
{
    // create a mask where the area we want is black and the rest is transparent
    QImage imageMask( r.size(), QImage::Format_ARGB32 );
    {
        imageMask.fill( Qt::transparent );
        QPainter painter( &imageMask );

        QBrush brush( Qt::black );
        brush.setStyle( Qt::SolidPattern );
        painter.setBrush( brush );

        painter.setPen( QPen( Qt::NoPen ) );
        painter.drawEllipse( QRect( 0, 0, r.width(), r.height() ) );
    }
    return imageMask;
}

void PSelectEllipse::doPaint( QPainter *pPainter, bool )
{
    //
    pPainter->setPen( QPen( Qt::DashLine ) );

    // paint
    pPainter->drawEllipse( r.normalized() );                         
}



