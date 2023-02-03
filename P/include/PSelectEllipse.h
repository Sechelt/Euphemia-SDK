#ifndef H_PSelectEllipse
#define H_PSelectEllipse

#include "PSelectRectangle.h"

class PSelectEllipse : public PSelectRectangle
{
    Q_OBJECT
public:
    PSelectEllipse( PCanvas *pCanvas );

protected:
    virtual void doPaint( QPainter * ) override;
};

#endif
