#ifndef H_PHandle
#define H_PHandle

#include "PGraphicsView.h"

/*!
 * \brief Handle used to allow User to manipulate lines/shapes. 
 *  
 * The handles are a child of the current PGraphivsView::viewport. 
 *  
 * This means that they avoid being scaled either in the main viewport 
 * or a minimap viewport. The downside is that there is more coordinate 
 * mapping. 
 *  
 * The coordinate mapping is done by the PCanvas. The handle simply 
 * assumes all coordinates are relative to its parent - the viewport. 
 *  
 * The handle does not even care about its mouse events. It relies on 
 * the canvas to manage it position. 
 *  
 * \author pharvey (2/10/23)
 */
class PHandle : public QWidget
{
    Q_OBJECT
public:
    enum Type
    {
        TypeDrag,
        TypeMovePoint,
        TypeNewPoint,
        TypeSizeLeft,
        TypeSizeRight,
        TypeSizeTop,
        TypeSizeBottom,
        TypeSizeTopRight,
        TypeSizeTopLeft,
        TypeSizeBottomRight,
        TypeSizeBottomLeft
    };

    PHandle( PGraphicsView *pView, Type nType, const QPoint &pointCenter );

    void paintEvent( QPaintEvent *pEvent );

    void setType( Type );
    void setCenter( const QPoint & );

    Type    getType()   { return nType;         }
    QPoint  getCenter() { return geometry().center();    }

    void doMoveBy( int nX, int nY );
    void doMoveBy( const QPoint & );

protected:
    Type            nType;
    PGraphicsView * pView;
};

#endif

