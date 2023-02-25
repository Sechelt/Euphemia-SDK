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

#ifndef H_PHandle
#define H_PHandle

#include "PGraphicsView.h"

/*!
 * \brief Handle used to allow User to manipulate lines/shapes. 
 *  
 * The handles are a child of the current PGraphivsView::viewport. 
 *  
 * This means that they avoid being scaled either in the main viewport 
 * or a minimap viewport. The downside is that there is more coordinate 
 * mapping. 
 *  
 * The coordinate mapping is done by the PCanvas. The handle simply 
 * assumes all coordinates are relative to its parent - the viewport. 
 *  
 * The handle does not even care about its mouse events. It relies on 
 * the canvas to manage it position. 
 *  
 * \author pharvey (2/10/23)
 */
class PHandle : public QWidget
{
    Q_OBJECT
public:
    enum Type
    {
        TypeDrag,
        TypeMovePoint,
        TypePointFactory,
        TypeSizeLeft,
        TypeSizeRight,
        TypeSizeTop,
        TypeSizeBottom,
        TypeSizeTopRight,
        TypeSizeTopLeft,
        TypeSizeBottomRight,
        TypeSizeBottomLeft,
        TypeFillSeed,
        TypeGradientStart,
        TypeGradientStop,
        TypeGradientIntermediate,
        TypeGradientRadius,
        TypeGradientFocal,
        TypeGradientAngle
    };

    // mostly to set a background shape
    enum Shape
    {
        ShapeCircle,
        ShapeSquare
    };

    PHandle( PGraphicsView *pView, Type nType, const QPoint &pointCenter, Shape nShape = ShapeCircle, const QColor &color = QColor( Qt::green ) );

    void paintEvent( QPaintEvent *pEvent );

    void setType( Type );
    void setCenter( const QPoint & );

    Type    getType()   { return nType;         }
    QPoint  getCenter() { return geometry().center();    }

    void doMoveBy( int nX, int nY );
    void doMoveBy( const QPoint & );

protected:
    Type            nType   = TypeMovePoint;
    Shape           nShape  = ShapeCircle;
    QColor          color   = QColor( Qt::green );
    PGraphicsView * pView;
};

#endif

