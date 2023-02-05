#ifndef H_PDrawErase
#define H_PDrawErase

#include "PFreeBase.h"

class PDrawErase : public PFreeBase
{
public:
    PDrawErase( PCanvas *pCanvas );

    virtual QRect doPress( QMouseEvent *pEvent ) override;
    virtual QRect doMove( QMouseEvent *pEvent ) override;
    virtual QRect doRelease( QMouseEvent *pEvent ) override;

protected:
    QRect doDrawShape( const QPoint & );
    void  doFill( const QRect &rect, const QImage &imageMask, QImage *pImageCanvas, const QColor &colorMask );
};

class PEraseToolBar : public QWidget
{
public:
    PEraseToolBar( QWidget *p );
};

#endif
