#ifndef H_PDrawRectangleFilled
#define H_PDrawRectangleFilled

#include "PDrawRectangle.h"

class PDrawRectangleFilled : public PDrawRectangle
{
    Q_OBJECT
public:
    PDrawRectangleFilled( PCanvas *pCanvas );

protected:
    virtual void doPaint( QPainter * ) override;
};

#endif
