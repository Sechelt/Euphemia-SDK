#ifndef H_PDrawRectangle
#define H_PDrawRectangle

#include "PShapeBase.h"

class PDrawRectangle : public PShapeBase
{
    Q_OBJECT
public:
    PDrawRectangle( PCanvas *pCanvas );
    virtual ~PDrawRectangle();

    virtual QRect  getRect() override { return r.normalized(); }
    virtual QImage getCopy() override;

    virtual void doDoubleClick( PMouseEvent *pEvent ) override;
    virtual void doPress( PMouseEvent *pEvent ) override;
    virtual void doMove( PMouseEvent *pEvent ) override; 
    virtual void doRelease( PMouseEvent *pEvent ) override;
    virtual void doCommit() override;

protected:
    QRect r;

    virtual void doPaint( QPainter *, bool bCommit = true );
    virtual void doDrawState( const QPoint & ) override;
    virtual void doManipulateState() override;
    virtual void doIdleState() override;
    virtual void doCreateHandles() override;
    virtual void doSyncHandles() override;
    virtual void doMoveHandle( const QPoint &pointPos );
    virtual void doSyncHandleTypes();
};

#endif
