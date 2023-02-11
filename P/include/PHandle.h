#ifndef H_PHandle
#define H_PHandle

#include "P.h"

/*!
 * \brief Handle used to allow User to manipulate lines/shapes. 
 *  
 * Unlike; canvas, shapes, etc - this; 
 *  
 *      - is not the same size as scene/canvas/image (its smaller)
 *      - is purely in double precision
 *      - will try to maintain consistent size even during zoom by countering scale change
 * 
 * \author pharvey (2/10/23)
 */
class PHandle : public QGraphicsObject
{
    Q_OBJECT
public:
    enum Type
    {
        TypeDrag,
        TypeMovePoint,
        TypeSizeLeft,
        TypeSizeRight,
        TypeSizeTop,
        TypeSizeBottom,
        TypeSizeTopRight,
        TypeSizeTopLeft,
        TypeSizeBottomRight,
        TypeSizeBottomLeft
    };

    PHandle( Type nType, const QPointF &pointCenter );

    virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 ) override;
    virtual QRectF boundingRect() const override;

    void setType( Type );
    void setCenter( const QPointF & );

    Type    getType()   { return nType; }
    QPointF getCenter() { return r.center(); }

    void doMoveBy( qreal nX, qreal nY );
    void doMoveBy( const QPointF & );

protected:
    QRectF  r;
    Type    nType;
};

#endif

