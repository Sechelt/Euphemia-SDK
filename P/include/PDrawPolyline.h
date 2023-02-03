#ifndef H_PDrawPolyline
#define H_PDrawPolyline

#include "PShapeBase.h"

class PDrawPolyline : public PShapeBase
{
    Q_OBJECT
public:
    PDrawPolyline( PCanvas *pCanvas );
    virtual ~PDrawPolyline();

    virtual QRect   doDoubleClick( QMouseEvent * ) override;
    virtual QRect   doPress( QMouseEvent *pEvent ) override;
    virtual QRect   doMove( QMouseEvent *pEvent ) override; 
    virtual QRect   doRelease( QMouseEvent *pEvent ) override;
    virtual QRect   doCommit() override;

protected:
    QPolygon polygon;

    virtual void paintEvent( QPaintEvent *pEvent ) override;

    virtual void doPaint( QPainter * );
    virtual void doDraw( const QPoint & );
    virtual void doManipulate();
    virtual void doIdle();
    virtual void doCreateHandles();
    virtual void doMoveHandle( const QPoint &pointPos );
};

class PPolylineToolBar : public QWidget
{
public:
    PPolylineToolBar( QWidget *p );
};

#endif
