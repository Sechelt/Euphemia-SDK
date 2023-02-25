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

#ifndef H_PGraphicsScene
#define H_PGraphicsScene

#include "PBackground.h"

class PCanvas;

/*!
 * \brief A graphics scene.
 *  
 * \author pharvey (2/8/23)
 */
class PGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    PGraphicsScene( const QRectF &r, QObject *pObject = nullptr );
    ~PGraphicsScene();

    PBackground *   getBackground() { return pBackground;   }
    PCanvas *       getCanvas()     { return pCanvas;       }

protected:
    PBackground *   pBackground;
    PCanvas *       pCanvas;

    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *pEvent );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *pEvent );
    virtual void mousePressEvent( QGraphicsSceneMouseEvent *pEvent );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *pEvent );
};

#endif


