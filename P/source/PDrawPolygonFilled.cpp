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
#include "PDrawPolygonFilled.h"

#include "PCanvas.h"

PDrawPolygonFilled::PDrawPolygonFilled( PCanvas *pCanvas )
    : PDrawPolygon( pCanvas )
{
}

void PDrawPolygonFilled::doPaint( QPainter *pPainter, bool )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setBrush( g_Context->getBrush() );

    // paint
    QPainterPath path;

    if ( nState == StateDraw )
    {
        QPolygon poly = polygon;
        poly.append( pointMouse );
        path.addPolygon( poly );
        pPainter->drawPolygon( poly, g_Context->getPolygonFilled().nFillRule );
    }
    else
    {
        path.addPolygon( polygon );
        pPainter->drawPolygon( polygon, g_Context->getPolygonFilled().nFillRule );
    }

    pPainter->fillPath( path, g_Context->getBrush() );
}

//
// PPolygonFilledToolBar
//
PPolygonFilledToolBar::PPolygonFilledToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pFillRule = new QComboBox( this );
    pFillRule->addItem( "Odd/Even Fill", Qt::OddEvenFill );
    pFillRule->addItem( "Winding Fill", Qt::WindingFill );
    pFillRule->setCurrentIndex( pFillRule->findData( g_Context->getPolygonFilled().nFillRule ) );
    pLayout->addWidget( pFillRule );
    connect( pFillRule, SIGNAL(activated(int)), SLOT(slotFillRule(int)) );

    pLayout->addStretch( 10 );

    connect( g_Context, SIGNAL(signalModified(const PContextPolygonFilled &)), SLOT(slotRefresh(const PContextPolygonFilled &)) );
}

void PPolygonFilledToolBar::slotRefresh( const PContextPolygonFilled &t )
{
    pFillRule->setCurrentIndex( pFillRule->findData( t.nFillRule ) );
}

void PPolygonFilledToolBar::slotFillRule( int n )
{
    PContextPolygonFilled polygonfilled = g_Context->getPolygonFilled();
    polygonfilled.nFillRule = (Qt::FillRule)pFillRule->itemData( n ).toInt();
    g_Context->setPolygonFilled( polygonfilled );
}

