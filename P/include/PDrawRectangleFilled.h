#ifndef H_PDrawRectangleFilled
#define H_PDrawRectangleFilled

#include "PDrawRectangle.h"

class PDrawRectangleFilled : public PDrawRectangle
{
    Q_OBJECT
public:
    PDrawRectangleFilled( PCanvas *pCanvas, const QPoint &pointBegin );

protected:
    virtual void doPaint( QPainter *, const QPoint &pointBegin, const QPoint &pointEnd ) override;
};

#endif
