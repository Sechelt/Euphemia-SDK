#ifndef H_PCanvasView
#define H_PCanvasView

#include "PCanvas.h"

class PCanvasView : public QScrollArea
{
    Q_OBJECT
public:
    PCanvasView( QWidget *parent, PCanvas *pCanvas );

    PCanvas *getCanvas() { return pCanvas; }

/*
signals:
    void signalPos( const QPoint & );

protected:
    void mousePressEvent( QMouseEvent *pEvent ) override;
    void mouseMoveEvent( QMouseEvent *pEvent ) override;
    void mouseReleaseEvent( QMouseEvent *pEvent ) override;
*/
    PCanvas *pCanvas;
};

#endif

