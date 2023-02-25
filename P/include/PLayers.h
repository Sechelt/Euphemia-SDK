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

#ifndef H_PLayers
#define H_PLayers

#include "PBackground.h"
#include "PCanvas.h"

/*!
 * \brief A stack of \sa PCanvas widgets with a \sa PBackground always at the back. 
 *  
 * re. stacking order 
 *  
 * This will always have exactly 1 PBackground and it will always be at the back. 
 * This will have 1-n PCanvas layered on the PBackground - so there will always be a 'current' canvas. 
 *  
 * re. size 
 *  
 * This will harmonize size of PBackground and all of the PCanvas's - they should be same size.
 *  
 * \author pharvey (2/5/23)
 */
class PLayers : public QWidget
{
public:
    PLayers( QWidget *parent, const QSize &size = QSize( 1024, 768 ) );

    PCanvas *getCanvas() { return vectorCanvas.at( nCurrent ); }

protected:
    PBackground *       pBackground;            /*!< only used to indicate transparency - default is transparent tile   */
    QVector<PCanvas *>  vectorCanvas;           /*!< 1-n                                                                */
    int                 nCurrent        = 0;        

    void resizeEvent( QResizeEvent *pEvent ) override;
};

#endif
