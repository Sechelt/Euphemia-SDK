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
#include "PDrawPolygon.h"

#include "PCanvas.h"
#include "PPenToolBar.h"

PDrawPolygon::PDrawPolygon( PCanvas *pCanvas )
    : PDrawPolyline( pCanvas )
{
}

/*!
 * \brief Paint the polygon. 
 *  
 * \author pharvey (2/1/23)
 * 
 * \param pPainter 
 * \param polygon 
 */
void PDrawPolygon::doPaint( QPainter *pPainter, bool )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );

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

//
// PPolygonToolBar
//
PPolygonToolBar::PPolygonToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pLayout->addWidget( new PPenToolBar( this ) );
    pLayout->addStretch( 10 );
}

