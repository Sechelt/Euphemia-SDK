#ifndef H_PDrawEllipse
#define H_PDrawEllipse

#include "PShapeBase.h"

class PDrawEllipse : public PShapeBase
{
    Q_OBJECT
public:
    PDrawEllipse( PCanvas *pCanvas, const QPoint &pointBegin );

    bool        doPress( QMouseEvent *pEvent ) override;
    bool        doMove( QMouseEvent *pEvent ) override; 
    bool        doRelease( QMouseEvent *pEvent ) override;
    void        doCommit() override;

protected:
    QPoint pointBegin;
    QPoint pointEnd;

    void paintEvent( QPaintEvent *pEvent ) override;

    void doPaint( QPainter *, const QPoint &pointBegin, const QPoint &pointEnd );
    void doCreateHandles();
    void doMoveHandle( const QPoint &pointPos );
};

#endif
