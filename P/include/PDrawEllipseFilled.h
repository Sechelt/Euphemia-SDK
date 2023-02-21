#ifndef H_PDrawEllipseFilled
#define H_PDrawEllipseFilled

#include "PDrawEllipse.h"

class PDrawEllipseFilled : public PDrawEllipse
{
    Q_OBJECT
public:
    PDrawEllipseFilled( PCanvas *pCanvas );

protected:
    void doPaint( QPainter *, bool bCommit = true ) override;
};

#endif
