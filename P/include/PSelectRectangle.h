#ifndef H_PSelectRectangle
#define H_PSelectRectangle

#include "PDrawRectangle.h"

class PSelectRectangle : public PDrawRectangle
{
    Q_OBJECT
public:
    PSelectRectangle( PCanvas *pCanvas, bool ball = false );

    virtual QImage getCopy() override;

    virtual void doRelease( PMouseEvent *pEvent ) override;
    virtual void doCommit() override;
    virtual void doCut() override;

    virtual bool canCommit() override   { return false; }
    virtual bool canCut() override      { return ( nState == StateManipulate ); }

    virtual bool isSelector() override { return true; }

protected:
    virtual void doPaint( QPainter * ) override;
};

#endif
