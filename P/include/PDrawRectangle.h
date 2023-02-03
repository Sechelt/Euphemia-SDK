#ifndef H_PDrawRectangle
#define H_PDrawRectangle

#include "PShapeBase.h"

class PDrawRectangle : public PShapeBase
{
    Q_OBJECT
public:
    PDrawRectangle( PCanvas *pCanvas );
    virtual ~PDrawRectangle();

    virtual QRect   doDoubleClick( QMouseEvent * ) override;
    virtual QRect   doPress( QMouseEvent *pEvent ) override;
    virtual QRect   doMove( QMouseEvent *pEvent ) override; 
    virtual QRect   doRelease( QMouseEvent *pEvent ) override;
    virtual QRect   doCommit() override;

protected:
    QRect r;

    virtual void paintEvent( QPaintEvent *pEvent ) override;

    virtual void doPaint( QPainter * );
    virtual void doDraw( const QPoint & );
    virtual void doManipulate();
    virtual void doIdle();
    virtual void doCreateHandles();
    virtual void doMoveHandle( const QPoint &pointPos );
    virtual void doSyncHandleTypes();
};

class PRectangleToolBar : public QWidget
{
public:
    PRectangleToolBar( QWidget *p );
};

#endif
