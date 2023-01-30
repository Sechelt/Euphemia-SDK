#ifndef H_PSelectEllipse
#define H_PSelectEllipse

#include "PDrawEllipse.h"

class PSelectEllipse : public PDrawEllipse
{
    Q_OBJECT
public:
    PSelectEllipse( PCanvas *pCanvas, const QPoint &pointBegin );

protected:
    virtual void doPaint( QPainter *, const QPoint &pointBegin, const QPoint &pointEnd ) override;
};

#endif
