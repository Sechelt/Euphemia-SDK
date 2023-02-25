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
#include "PMagnifierSelection.h"

#include "PCanvas.h"

#define PMagnifierSelectionMove 0

PMagnifierSelection::PMagnifierSelection( PCanvas *pCanvas )
    : PShapeBase( pCanvas )
{
}

PMagnifierSelection::~PMagnifierSelection()
{
    doCancel();
}

void PMagnifierSelection::doDoubleClick( PMouseEvent *pEvent )
{ 
    Q_UNUSED( pEvent );
}

void PMagnifierSelection::doPress( PMouseEvent *pEvent )
{
    Q_ASSERT( !acceptHoverEvents() );

    if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        doDrawState( pEvent->pos() );
        doManipulateState();
        pHandle = getHandle( pEvent->pos() );
        break;
    case StateDraw:
        break;
    case StateManipulate:
        pHandle = getHandle( pEvent->pos() );
        break;
    }
}

void PMagnifierSelection::doMove( PMouseEvent *pEvent ) 
{
    Q_ASSERT( !acceptHoverEvents() );

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        break;
    case StateManipulate:
        if ( pHandle ) doMoveHandle( pEvent->pos() );
        break;
    }
}

void PMagnifierSelection::doRelease( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        break;
    case StateManipulate:
        pHandle = nullptr;
        break;
    }
}

void PMagnifierSelection::doCommit()
{
}

void PMagnifierSelection::doPaint( QPainter *pPainter, bool )
{
    //
    QPen pen( Qt::DashLine );
    pen.setColor( Qt:: darkGray );
    pPainter->setPen( pen );

    // paint
    pPainter->drawLine( r.topLeft(), r.topRight() );
    pPainter->drawLine( r.topLeft(), r.bottomLeft() );
}

void PMagnifierSelection::doDrawState( const QPoint &point )
{
    Q_ASSERT( nState == StateIdle );
    r = QRect( point, QSize( 50, 50 ) );
    nState = StateDraw;
    update();
    emit signalChangedState();
}

void PMagnifierSelection::doManipulateState()
{
    Q_ASSERT( nState == StateDraw );
    doCreateHandles();
    nState = StateManipulate;
    emit signalChangedState();
}

void PMagnifierSelection::doIdleState()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );

    if ( nState == StateDraw )
    {
        nState = StateIdle;
    }
    else if ( nState == StateManipulate )
    {
        doDeleteHandles();
        nState = StateIdle;
    }
    update();
    emit signalChangedState();
}

void PMagnifierSelection::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    // their parent will be the viewport so...
    QRect rectView = pView->mapFromScene( r ).boundingRect();

    // a single handle - just for moving rect around.
    // we use topleft instead of center so the view is less obscured.
    PHandle *pHandle = new PHandle( pView, PHandle::TypeDrag, rectView.topLeft() );
    vectorHandles.append( pHandle );
    pHandle->show();
}

void PMagnifierSelection::doSyncHandles()
{
    QRect rectView = pView->mapFromScene( r.normalized() ).boundingRect();

    vectorHandles[PMagnifierSelectionMove]->setCenter( rectView.topLeft() );
}

void PMagnifierSelection::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );

    // adjust our geometry
    if ( pHandle == vectorHandles[PMagnifierSelectionMove] )
    {
        r.moveTopLeft( pointPos );
        pHandle->setCenter( pView->mapFromScene( pointPos ) );
    }
    update();
    emit signalMoved();
}




