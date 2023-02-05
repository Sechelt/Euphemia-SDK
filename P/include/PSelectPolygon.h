#ifndef H_PSelectPolygon
#define H_PSelectPolygon

#include "PDrawPolygon.h"

class PSelectPolygon : public PDrawPolygon
{
    Q_OBJECT
public:
    PSelectPolygon( PCanvas *pCanvas );

    virtual QRect doCommit() override;
    virtual void  doCut() override;
    virtual void  doCopy() override;

    virtual bool canCommit() override;
    virtual bool canCut() override;
    virtual bool canCopy() override;

protected:
    virtual void doPaint( QPainter * );
    virtual QImage doTrim( QImage &image, const QImage &imageEllipse, bool bErase = false );
};

#endif
