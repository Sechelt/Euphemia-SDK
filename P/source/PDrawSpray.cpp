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
#include "PDrawSpray.h"

#include <WColorButton.h>

#include "PCanvas.h"

PDrawSpray::PDrawSpray( PCanvas *pCanvas )
    : PFreeBase( pCanvas )
{
}

void PDrawSpray::doPress( PMouseEvent *pEvent )
{
    doSpray( pEvent->pos() );
}

void PDrawSpray::doMove( PMouseEvent *pEvent )
{
    doSpray( pEvent->pos() );
}

void PDrawSpray::doRelease( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );
}

void PDrawSpray::doSpray( const QPoint &point )
{
    int nX = point.x();     
    int nY = point.y();     
    int nR = g_Context->getSpray().nRadius;
    int nN = g_Context->getSpray().nPoints;
    QPoint pointSpray;

    // draw points
    QPainter painter( g_Context->getImage() );
    painter.setPen( g_Context->getPen() );

    for ( int n = 0; n < nN; n++ )
    {
        // Get Angle in radians
        double nTheta = 2 * M_PI * getRandom();
        // Get length from center
        double nLen = qSqrt( getRandom() ) * nR;
 
        pointSpray.setX( nX + nLen  * qCos( nTheta ) );
        pointSpray.setY( nY + nLen  * qSin( nTheta ) );

        painter.drawPoint( pointSpray );
    }

    pCanvas->update();
}

double PDrawSpray::getRandom()
{
    // something between 0 and 1
    return (double)rand() / RAND_MAX;
}

//
// PSprayToolBar
//
PSprayToolBar::PSprayToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );

    //
    pRadius = new QSpinBox( this );
    pRadius->setMinimum( 1 );
    pRadius->setMaximum( 50 );
    pRadius->setValue( g_Context->getSpray().nRadius );
    pRadius->setToolTip( tr("radius") );
    pLayout->addWidget( pRadius );
    connect( pRadius, SIGNAL(valueChanged(int)), SLOT(slotRadius(int)) );
    //
    pPoints = new QSpinBox( this );
    pPoints->setMinimum( 1 );
    pPoints->setMaximum( 50 );
    pPoints->setValue( g_Context->getSpray().nPoints );
    pPoints->setToolTip( tr("points") );
    pLayout->addWidget( pPoints );
    connect( pPoints, SIGNAL(valueChanged(int)), SLOT(slotPoints(int)) );
    //
    pLayout->addStretch( 10 );

    connect( g_Context, SIGNAL(signalModified(const PContextSpray &)), SLOT(slotRefresh(const PContextSpray &)) );
}

void PSprayToolBar::slotRefresh( const PContextSpray &t )
{
    pRadius->setValue( t.nRadius );
    pPoints->setValue( t.nPoints );
}

void PSprayToolBar::slotRadius( int n )
{
    PContextSpray spray = g_Context->getSpray();
    spray.nRadius = n;
    g_Context->setSpray( spray );
}

void PSprayToolBar::slotPoints( int n )
{
    PContextSpray spray = g_Context->getSpray();
    spray.nPoints = n;
    g_Context->setSpray( spray );
}

