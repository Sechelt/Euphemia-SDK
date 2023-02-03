#ifndef H_PSelectRectangle
#define H_PSelectRectangle

#include "PDrawRectangle.h"

class PSelectRectangle : public PDrawRectangle
{
    Q_OBJECT
public:
    PSelectRectangle( PCanvas *pCanvas );

    virtual QRect doCommit() override;

    virtual bool canCommit() override;

protected:
    virtual void doPaint( QPainter * ) override;
};

#endif
