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

#ifndef H_PGraphicsView
#define H_PGraphicsView

#include <WZoomWidget.h>

#include "PGraphicsScene.h"

/*!
 * \brief A graphics view for PGraphicsScene. 
 *  
 * re. scale
 *  
 * We always scale the same on X & Y axis so we only use a single qreal.
 * Only use slotZoomChanged to set scale. 
 * Use getScale to get the single qreal.
 *  
 * re. zoom
 *  
 * This can work with a WZoomWidget. The WZoomWidget can be used to set the zoom 
 * by connecting to slotZoomChanged(). This will call setScale. 
 *  
 * getZoom() and getZoomFit() can be used to sync WZoomWidget when this 
 * view has become the active view.
 *  
 * \author pharvey (2/8/23)
 */
class PGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    PGraphicsView( PGraphicsScene *pScene, QWidget *pParent = nullptr );

    PGraphicsScene *        getScene();
    PCanvas *               getCanvas();
    qreal                   getScale();
    WZoomWidget::FitTypes   getZoomFit()    { return nZoomFit;  }
    int                     getZoom()       { return nZoom;     }

signals:
    void signalZoomChanged( WZoomWidget::FitTypes nFit, int nZoom );

public slots:
    void slotZoomChanged( WZoomWidget::FitTypes nFit, int nZoom );
 
protected:
    WZoomWidget::FitTypes   nZoomFit    = WZoomWidget::FitIgnore;
    int                     nZoom       = 100; // % inc/dec by 10

    void setScale( qreal n );
};

#endif


