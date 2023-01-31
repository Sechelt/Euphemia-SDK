#ifndef H_PDrawPolygon
#define H_PDrawPolygon

#include "PShapeBase.h"

class PDrawPolygon : public PShapeBase
{
    Q_OBJECT
public:
    PDrawPolygon( PCanvas *pCanvas, const QPoint &pointBegin );

    virtual bool doDoubleClick( QMouseEvent * ) override { return true; }
    virtual bool doPress( QMouseEvent *pEvent ) override;
    virtual bool doMove( QMouseEvent *pEvent ) override; 
    virtual bool doRelease( QMouseEvent *pEvent ) override;
    virtual void doCommit() override;

protected:
    QPoint pointBegin;
    QPoint pointEnd;

    virtual void paintEvent( QPaintEvent *pEvent ) override;

    virtual void doPaint( QPainter *, const QPoint &pointBegin, const QPoint &pointEnd );
    virtual void doCreateHandles();
    virtual void doMoveHandle( const QPoint &pointPos );
};

#endif
