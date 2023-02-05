#ifndef H_PPasteRectangle
#define H_PPasteRectangle

#include "PDrawRectangle.h"

class PPasteRectangle : public PDrawRectangle
{
    Q_OBJECT
public:
    PPasteRectangle( PCanvas *pCanvas );

    virtual QRect doPress( QMouseEvent *pEvent ) override;
    virtual QRect doCommit() override;

    virtual bool canCommit() override;

protected:
    QImage image;

    virtual void doPaint( QPainter * ) override;
    virtual void doDraw( const QPoint &point );
};

#endif
