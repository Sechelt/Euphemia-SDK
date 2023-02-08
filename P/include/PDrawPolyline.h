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

    virtual QRect   doDoubleClick( QMouseEvent * ) override;
    virtual QRect   doPress( QMouseEvent *pEvent ) override;
    virtual QRect   doMove( QMouseEvent *pEvent ) override; 
    virtual QRect   doRelease( QMouseEvent *pEvent ) override;
    virtual QRect   doCommit() override;

protected:
    QPolygon polygon;

    virtual void paintEvent( QPaintEvent *pEvent ) override;

    virtual void doPaint( QPainter * );
    virtual void doDraw( const QPoint & ) override;
    virtual void doManipulate() override;
    virtual void doIdle() override;
    virtual void doCreateHandles() override;
    virtual void doMoveHandle( const QPoint &pointPos );
};

#endif
