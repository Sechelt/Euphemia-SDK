#ifndef H_PSelectRectangle
#define H_PSelectRectangle

#include "PDrawRectangle.h"

class PSelectRectangle : public PDrawRectangle
{
    Q_OBJECT
public:
    PSelectRectangle( PCanvas *pCanvas, const QPoint &pointBegin );

protected:
    virtual void doPaint( QPainter *, const QPoint &pointBegin, const QPoint &pointEnd ) override;
};

#endif
