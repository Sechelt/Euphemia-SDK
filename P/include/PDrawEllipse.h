#ifndef H_PDrawEllipse
#define H_PDrawEllipse

#include "PDrawRectangle.h"

class PDrawEllipse : public PDrawRectangle
{
    Q_OBJECT
public:
    PDrawEllipse( PCanvas *pCanvas, const QPoint &pointBegin );

protected:
    virtual void doPaint( QPainter *, const QPoint &pointBegin, const QPoint &pointEnd );
};


class PEllipseToolBar : public QWidget
{
public:
    PEllipseToolBar( QWidget *p );
};

#endif
