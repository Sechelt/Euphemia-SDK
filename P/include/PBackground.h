#ifndef H_PBackground
#define H_PBackground

#include "P.h"

class PBackground : public QGraphicsObject
{
public:
    PBackground();

protected:
    virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 ) override;
    virtual QRectF boundingRect() const override;

private:
    QBrush brush;
};

#endif
