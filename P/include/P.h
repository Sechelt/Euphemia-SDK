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

#ifndef P_H
#define P_H

#include <QtGui>
#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>
#include <QtMath>

namespace P 
{

};

class PMouseEvent
{
public:
    PMouseEvent( QGraphicsSceneMouseEvent *p )
    {
        Q_ASSERT( p );
        nButton = p->button();
        nButtons = p->buttons();
        nModifiers = p->modifiers();
        pointPos = p->scenePos().toPoint();
    }

    Qt::MouseButton button() const              { return nButton;       }
    Qt::MouseButtons buttons() const            { return nButtons;      }
    Qt::KeyboardModifiers modifiers() const     { return nModifiers;    }
    QPoint pos() const                          { return pointPos;      }

// not needed - just use constructor
/*
    inline T& operator=(const T& other)    
    {                                      
        if ( this == &other ) return *this;
        nButton = other.button();          
        nButtons = other.buttons();        
        nModifiers = other.modifiers();    
        pointPos = other.pos();  
        return *this;                      
    }                                      
*/

protected:
    Qt::MouseButton         nButton;
    Qt::MouseButtons        nButtons; 
    Qt::KeyboardModifiers   nModifiers;
    QPoint                  pointPos;
};

#endif



