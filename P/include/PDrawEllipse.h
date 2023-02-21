#ifndef H_PDrawEllipse
#define H_PDrawEllipse

#include "PDrawRectangle.h"

class PDrawEllipse : public PDrawRectangle
{
    Q_OBJECT
public:
    PDrawEllipse( PCanvas *pCanvas );

protected:
    virtual void doPaint( QPainter *, bool bCommit = true );
};


#endif
