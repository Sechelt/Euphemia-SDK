#ifndef H_PDrawPolyline
#define H_PDrawPolyline

#include "PShapeBase.h"

class PDrawPolyline : public PShapeBase
{
    Q_OBJECT
public:
    PDrawPolyline( PCanvas *pCanvas );
    virtual ~PDrawPolyline();

    virtual QImage getCopy() override;

    virtual void doDoubleClick( PMouseEvent * ) override;
    virtual void doPress( PMouseEvent * ) override;
    virtual void doMove( PMouseEvent * ) override; 
    virtual void doRelease( PMouseEvent * ) override;
    virtual void doCommit() override;

protected:
    QPolygon polygon;

    virtual void doPaint( QPainter * );
    virtual void doDrawState( const QPoint & ) override;
    virtual void doManipulateState() override;
    virtual void doIdleState() override;
    virtual void doCreateHandles() override;
    virtual void doMoveHandle( const QPoint &pointPos );
};

#endif
