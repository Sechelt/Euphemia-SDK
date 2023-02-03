#ifndef H_PHandle
#define H_PHandle

#include "P.h"

class PHandle : public QWidget
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

    PHandle( QWidget *pParent, Type nType, const QPoint &pointCenter );

    void setType( Type );
    void setCenter( const QPoint & );

    Type    getType()   { return nType; }
    QPoint  getCenter() { return geometry().center(); }

    void doMoveBy( int nX, int nY );
    void doMoveBy( const QPoint & );

protected:
    Type nType;

    void paintEvent( QPaintEvent *pEvent ) override;

};

#endif

