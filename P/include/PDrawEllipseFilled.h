#ifndef H_PDrawEllipseFilled
#define H_PDrawEllipseFilled

#include "PDrawEllipse.h"

class PDrawEllipseFilled : public PDrawEllipse
{
    Q_OBJECT
public:
    PDrawEllipseFilled( PCanvas *pCanvas, const QPoint &pointBegin );

protected:
    virtual void doPaint( QPainter *, const QPoint &pointBegin, const QPoint &pointEnd ) override;
};

class PEllipseFilledToolBar : public QWidget
{
public:
    PEllipseFilledToolBar( QWidget *p );
};

#endif
