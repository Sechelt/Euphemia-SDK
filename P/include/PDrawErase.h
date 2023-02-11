#ifndef H_PDrawErase
#define H_PDrawErase

#include "PFreeBase.h"

class PDrawErase : public PFreeBase
{
public:
    PDrawErase( PCanvas *pCanvas );

    void doPress( PMouseEvent *pEvent ) override;
    void doMove( PMouseEvent *pEvent ) override;
    void doRelease( PMouseEvent *pEvent ) override;

protected:
    void doDrawShape( const QPoint & );
    void doFill( const QRect &rect, const QImage &imageMask, QImage *pImageCanvas, const QColor &colorMask );
};

class PEraseToolBar : public QWidget
{
public:
    PEraseToolBar( QWidget *p );
};

#endif
