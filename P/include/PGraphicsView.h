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


