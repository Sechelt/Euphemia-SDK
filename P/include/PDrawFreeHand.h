#ifndef H_PDrawFreeHand
#define H_PDrawFreeHand

#include "PFreeBase.h"

class PDrawFreeHand : public PFreeBase
{
public:
    PDrawFreeHand( PCanvas *pCanvas );

    virtual QRect doPress( QMouseEvent *pEvent ) override;
    virtual QRect doMove( QMouseEvent *pEvent ) override;
    virtual QRect doRelease( QMouseEvent *pEvent ) override;

protected:
    PCanvas *   pCanvas;
    QPoint      pointLast;

    QRect doDrawLine( const QPoint & );
};

class PFreeHandToolBar : public QWidget
{
public:
    PFreeHandToolBar( QWidget *p );
};

#endif
