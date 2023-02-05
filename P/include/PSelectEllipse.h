#ifndef H_PSelectEllipse
#define H_PSelectEllipse

#include "PSelectRectangle.h"

class PSelectEllipse : public PSelectRectangle
{
    Q_OBJECT
public:
    PSelectEllipse( PCanvas *pCanvas );

    virtual void  doCut() override;
    virtual void  doCopy() override;

protected:
    virtual void doPaint( QPainter * ) override;
    virtual QImage doTrim( QImage &image, const QImage &imageEllipse, bool bErase = false );
};

#endif
