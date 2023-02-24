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

