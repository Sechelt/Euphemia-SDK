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
#include "PSelectRectangle.h"

PSelectRectangle::PSelectRectangle( PCanvas *pCanvas, bool bAll )
    : PDrawRectangle( pCanvas )
{
    if ( bAll )
    {
        // fake doDraw to cover all
        r = QRect( QPoint( 0, 0 ), g_Context->getImage()->size() );
        nState = StateDraw;
        // go straight into manipulate
        doManipulateState();
    }
}

/*!
 * \brief Return a copy of the SELECTED AREA.
 * 
 * \author pharvey (2/6/23)
 * 
 * \return QImage 
 */
QImage PSelectRectangle::getCopy()
{
    return g_Context->getImage()->copy( r );
}

// this removes the auto commit
void PSelectRectangle::doRelease( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        doManipulateState();
        break;
    case StateManipulate:
        pHandle = nullptr;
        break;
    }
}

void PSelectRectangle::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );
    // no commit for a select shape - we just go straight to idle
    doIdleState();
}

void PSelectRectangle::doCut()
{
    QImage *pImage = g_Context->getImage();

    // copy
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setImage( pImage->copy( r ) );
    // delete (drawRect() with a transparent brush is a NoOp) so scan pixels
    QPainter painter( pImage );
    QColor colorBackground( Qt::transparent );      // this does not set alpha for some reason
    colorBackground.setAlpha( 0 );                  // so we set here
    r = r.normalized();
    qreal nLeft       = r.left() + 1;
    qreal nTop        = r.top() + 1;
    qreal nRight      = r.right() - 1;
    qreal nBottom     = r.bottom() - 1;

    for ( qreal nX = nLeft; nX <= nRight; nX++ )
    {
        for ( qreal nY = nTop; nY <= nBottom; nY++ )
        {
            pImage->setPixelColor( nX, nY, colorBackground );
        }
    }
}

void PSelectRectangle::doPaint( QPainter *pPainter, bool )
{
    //
    pPainter->setPen( QPen( Qt::DashLine ) );

    // paint
    pPainter->drawRect( r.normalized() );                         
}

