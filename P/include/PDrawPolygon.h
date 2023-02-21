#ifndef H_PDrawPolygon
#define H_PDrawPolygon

#include "PDrawPolyline.h"

class PDrawPolygon : public PDrawPolyline
{
    Q_OBJECT
public:
    PDrawPolygon( PCanvas *pCanvas );

protected:
    virtual void doPaint( QPainter *, bool bCommit = true ) override;
};

class PPolygonToolBar : public QWidget
{
public:
    PPolygonToolBar( QWidget *p );
};

#endif
