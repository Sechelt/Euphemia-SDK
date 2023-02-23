#ifndef H_PMagnifierSelection
#define H_PMagnifierSelection

#include "PShapeBase.h"

class PMagnifierSelection : public PShapeBase
{
    Q_OBJECT
public:
    PMagnifierSelection( PCanvas *pCanvas );
    virtual ~PMagnifierSelection();

    virtual QRect  getRect() override { return r.normalized(); }

    virtual void doDoubleClick( PMouseEvent *pEvent ) override;
    virtual void doPress( PMouseEvent *pEvent ) override;
    virtual void doMove( PMouseEvent *pEvent ) override; 
    virtual void doRelease( PMouseEvent *pEvent ) override;
    virtual void doCommit() override;

    virtual bool    canCommit() override { return false; }
    virtual bool    canCut()    override { return false; }
    virtual bool    canCopy()   override { return false; }

signals:
    void signalMoved();

protected:
    QRect r;

    virtual void doPaint( QPainter *, bool bCommit = true );
    virtual void doDrawState( const QPoint & ) override;
    virtual void doManipulateState() override;
    virtual void doIdleState() override;
    virtual void doCreateHandles() override;
    virtual void doSyncHandles() override;
    virtual void doMoveHandle( const QPoint &pointPos );
};

#endif
