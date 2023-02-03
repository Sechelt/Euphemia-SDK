#ifndef H_PDrawEllipseFilled
#define H_PDrawEllipseFilled

#include "PDrawEllipse.h"

class PDrawEllipseFilled : public PDrawEllipse
{
    Q_OBJECT
public:
    PDrawEllipseFilled( PCanvas *pCanvas );

protected:
    virtual void doPaint( QPainter * ) override;
};

class PEllipseFilledToolBar : public QWidget
{
public:
    PEllipseFilledToolBar( QWidget *p );
};

#endif
