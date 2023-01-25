#ifndef H_PHandle
#define H_PHandle

#include "P.h"

class PHandle : public QWidget
{
    Q_OBJECT
public:
    enum Type
    {
        TypeMove,
        TypeMovePoint,
        TypeTopLeft,
        TypeTopRight,
        TypeBottomLeft,
        TypeBottomRight,
        TypeSizeH,
        TypeSizeV,
        TypeSizeHV
    };

    PHandle( QWidget *pParent, Type nType, const QPoint &pointCenter );

    void setCenter( const QPoint & );

    Type    getType()   { return nType; }
    QPoint  getCenter() { return geometry().center(); }

    void doMoveBy( int nX, int nY );

protected:
    void paintEvent( QPaintEvent *pEvent ) override;

    Type    nType;
    QPen    pen;
    QBrush  brush;
};

#endif

