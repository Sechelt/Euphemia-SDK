#ifndef H_PDrawPolygon
#define H_PDrawPolygon

#include "PShapeBase.h"

class PDrawPolygon : public PShapeBase
{
    Q_OBJECT
public:
    PDrawPolygon( PCanvas *pCanvas, const QPoint &pointBegin );

    virtual bool doDoubleClick( QMouseEvent * ) override;
    virtual bool doPress( QMouseEvent *pEvent ) override;
    virtual bool doMove( QMouseEvent *pEvent ) override; 
    virtual bool doRelease( QMouseEvent *pEvent ) override;
    virtual void doCommit() override;

protected:
    bool        bManipulating = false;  /*!< we start in 'drawing mode' and switch to 'manipulating mode' when double click.    */
    QPolygon    polygon;                /*!< in canvas coordinates                                                              */

    virtual void paintEvent( QPaintEvent *pEvent ) override;

    virtual void doPaint( QPainter *, const QPolygon & );
    virtual void doCreateHandles();
    virtual void doMoveHandle( const QPoint &pointPos );
};

class PPolygonToolBar : public QWidget
{
public:
    PPolygonToolBar( QWidget *p );
};

#endif
