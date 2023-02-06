#ifndef H_PSelectEllipse
#define H_PSelectEllipse

#include "PSelectRectangle.h"

class PSelectEllipse : public PSelectRectangle
{
    Q_OBJECT
public:
    PSelectEllipse( PCanvas *pCanvas );

    virtual QImage getCopy() override;

    virtual void  doCut() override;
    virtual void  doCopy() override;

protected:
    virtual QImage getMask() override;

    virtual void doPaint( QPainter * ) override;
};

#endif
