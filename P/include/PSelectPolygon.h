#ifndef H_PSelectPolygon
#define H_PSelectPolygon

#include "PDrawPolygon.h"

class PSelectPolygon : public PDrawPolygon
{
    Q_OBJECT
public:
    PSelectPolygon( PCanvas *pCanvas );

    virtual QImage getCopy() override;

    virtual QRect doDoubleClick( QMouseEvent * ) override;
    virtual QRect doCommit() override;
    virtual void  doCut() override;

    virtual bool canCommit() override   { return false; }
    virtual bool canCut() override      { return ( nState == StateManipulate ); }

    virtual bool isSelector() override { return true; }

protected:
    virtual QImage getMask() override;

    virtual void doPaint( QPainter * ) override;

};

#endif
