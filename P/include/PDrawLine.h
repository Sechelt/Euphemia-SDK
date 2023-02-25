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

#ifndef H_PDrawLine
#define H_PDrawLine

#include "PShapeBase.h"

class PDrawLine : public PShapeBase
{
    Q_OBJECT
public:
    PDrawLine( PCanvas *pCanvas );
    virtual ~PDrawLine();

    virtual QRect  getRect() override { return QRect( pointBegin, pointEnd ).normalized(); }
    virtual QImage getCopy() override;

    virtual void doDoubleClick( PMouseEvent *pEvent ) override;
    virtual void doPress( PMouseEvent *pEvent ) override;
    virtual void doMove( PMouseEvent *pEvent ) override; 
    virtual void doRelease( PMouseEvent *pEvent ) override;
    virtual void doCommit() override;

protected:
    QPoint pointBegin;
    QPoint pointEnd;

    virtual void doPaint( QPainter *, bool bCommit = true );

    virtual void doDrawState( const QPoint & ) override;
    virtual void doManipulateState() override;
    virtual void doIdleState() override;

    virtual void doCreateHandles() override;
    virtual void doSyncHandles() override;
    virtual void doMoveHandle( const QPoint &pointPos );
};

#endif
