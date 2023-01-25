#ifndef H_PDrawLine
#define H_PDrawLine

#include "PShapeBase.h"

class PDrawLine : public PShapeBase
{
    Q_OBJECT
public:
    PDrawLine( PCanvas *pCanvas, const QPoint &pointBegin );

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
