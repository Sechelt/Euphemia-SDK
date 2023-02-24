#ifndef H_PPanner
#define H_PPanner

#include "PGraphicsScene.h"

class PPanner : public QGraphicsView
{
    Q_OBJECT
public:
    PPanner( PGraphicsScene *pScene, QWidget *pParent );
    ~PPanner();

    QGraphicsView *getView(); // gets the main view

public slots:
    virtual void slotUpdate();

protected:
    bool bPanning;
    QPoint pointLast;

    // events
    void resizeEvent( QResizeEvent *pEvent );
    void mousePressEvent( QMouseEvent *pMouseEvent );
    void mouseMoveEvent( QMouseEvent *pMouseEvent );
    void mouseReleaseEvent( QMouseEvent *pMouseEvent );

    void drawForeground( QPainter *pPainter, const QRectF &rect );
    // void drawBackground( QPainter *pPainter, const QRectF &rect );
};

#endif


