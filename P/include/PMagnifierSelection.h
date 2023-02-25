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

#ifndef H_PMagnifierSelection
#define H_PMagnifierSelection

#include "PShapeBase.h"

class PMagnifierSelection : public PShapeBase
{
    Q_OBJECT
public:
    PMagnifierSelection( PCanvas *pCanvas );
    virtual ~PMagnifierSelection();

    virtual QRect  getRect() override { return r.normalized(); }

    virtual void doDoubleClick( PMouseEvent *pEvent ) override;
    virtual void doPress( PMouseEvent *pEvent ) override;
    virtual void doMove( PMouseEvent *pEvent ) override; 
    virtual void doRelease( PMouseEvent *pEvent ) override;
    virtual void doCommit() override;

    virtual bool    canCommit() override { return false; }
    virtual bool    canCut()    override { return false; }
    virtual bool    canCopy()   override { return false; }

signals:
    void signalMoved();

protected:
    QRect r;

    virtual void doPaint( QPainter *, bool bCommit = true );
    virtual void doDrawState( const QPoint & ) override;
    virtual void doManipulateState() override;
    virtual void doIdleState() override;
    virtual void doCreateHandles() override;
    virtual void doSyncHandles() override;
    virtual void doMoveHandle( const QPoint &pointPos );
};

#endif
