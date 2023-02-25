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

#ifndef H_PMagnifierWidget
#define H_PMagnifierWidget

#include "PGraphicsView.h"
#include "PShapeBase.h"

class PMagnifierWidget : public QWidget
{
    Q_OBJECT
public:
    PMagnifierWidget( QWidget *pParent );

    void setView( PGraphicsView *p );

protected:
    // set in setView()
    PGraphicsView * pView   = nullptr;
    PCanvas *       pCanvas = nullptr;
    QImage *        pImage  = nullptr;
    // set in paintEvent()
    PShapeBase *    pShape  = nullptr;
    int nXImage;
    int nYImage;
    int nSize       = 16;
    int nXCells;
    int nYCells;
    // set in mouse events
    QPen    pen;

    void mousePressEvent( QMouseEvent *pEvent ) override;
    void mouseMoveEvent( QMouseEvent *pEvent ) override;
    void mouseReleaseEvent( QMouseEvent *pEvent ) override;
    void paintEvent( QPaintEvent *pEvent ) override;
    void wheelEvent( QWheelEvent *pEvent ) override;

    void doCalcMatrix();

    QPoint mapCellToScene( const QPoint & );
    QPoint mapWidgetToCell( const QPoint & );
};

#endif 

