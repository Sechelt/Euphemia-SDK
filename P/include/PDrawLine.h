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

    virtual QRect doDoubleClick( QMouseEvent * ) override;
    virtual QRect doPress( QMouseEvent *pEvent ) override;
    virtual QRect doMove( QMouseEvent *pEvent ) override; 
    virtual QRect doRelease( QMouseEvent *pEvent ) override;
    virtual QRect doCommit() override;

protected:
    QPoint pointBegin;
    QPoint pointEnd;

    virtual void paintEvent( QPaintEvent *pEvent ) override;

    virtual void doPaint( QPainter * );
    virtual void doDraw( const QPoint & ) override;
    virtual void doManipulate() override;
    virtual void doIdle() override;
    virtual void doCreateHandles() override;
    virtual void doMoveHandle( const QPoint &pointPos );
};

class PLineToolBar : public QWidget
{
public:
    PLineToolBar( QWidget *p );
};

#endif
