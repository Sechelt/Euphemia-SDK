#ifndef H_PDrawLine
#define H_PDrawLine

#include "PShapeBase.h"

class PDrawLine : public PShapeBase
{
    Q_OBJECT
public:
    PDrawLine( PCanvas *pCanvas );
    virtual ~PDrawLine();

    virtual QImage getCopy() override;

    virtual void doDoubleClick( PMouseEvent *pEvent ) override;
    virtual void doPress( PMouseEvent *pEvent ) override;
    virtual void doMove( PMouseEvent *pEvent ) override; 
    virtual void doRelease( PMouseEvent *pEvent ) override;
    virtual void doCommit() override;

protected:
    QPoint pointBegin;
    QPoint pointEnd;

    virtual void doPaint( QPainter * );

    virtual void doDrawState( const QPoint & ) override;
    virtual void doManipulateState() override;
    virtual void doIdleState() override;

    virtual void doCreateHandles() override;
    virtual void doSyncHandles() override;
    virtual void doMoveHandle( const QPoint &pointPos );
};

#endif
